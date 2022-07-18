/**
 * Copyright (c) 2022 Luca Erbetta
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "CameraController.h"

#include <chrono>
#include <filesystem>
#include <thread>

#include "events/EventBroker.h"

using std::dynamic_pointer_cast;
using namespace std::this_thread;
using namespace gphotow;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::filesystem::path;

CameraController::CameraController(string download_dir)
    : HSM(&CameraController::stateInit), download_dir(download_dir)
{
    sEventBroker.subscribe(this, TOPIC_CAMERA_CMD);
}

State CameraController::stateInit(const EventPtr& ev)
{
    return transition(&CameraController::stateSuper);
}

State CameraController::stateSuper(const EventPtr& ev)
{
    auto slog      = log.getChild("stateSuper");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            LOG_INFO(slog, "ENTRY");
            break;
        case EventSMInit::id:
            retState = transition(&CameraController::stateDisconnected);
            break;
        case EventSMExit::id:
            LOG_INFO(slog, "EXIT");
            break;
        case EventCameraCmdDownload::id:
        {
            auto d_ev = dynamic_pointer_cast<const EventCameraCmdDownload>(ev);
            do_download = d_ev->download;
            break;
        }
        default:
            retState = tran_super(&CameraController::Hsm_top);
            break;
    }
    return retState;
}

State CameraController::stateDisconnected(const EventPtr& ev)
{
    auto slog      = log.getChild("stateDisconnected");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            onStateChanged(CCState::DISCONNECTED);
            LOG_INFO(slog, "ENTRY");
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_INFO(slog, "EXIT");
            break;
        case EventCameraCmdConnect::id:
            if (connect())
                retState = transition(&CameraController::stateConnected);
            else
                retState = transition(&CameraController::stateConnectionError);

            break;
        default:
            retState = tran_super(&CameraController::stateSuper);
            break;
    }
    return retState;
}

State CameraController::stateConnected(const EventPtr& ev)
{
    auto slog      = log.getChild("stateConnected");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            onStateChanged(CCState::CONNECTED);

            sEventBroker.post(EventCameraConnected{}, TOPIC_CAMERA_EVENT);

            LOG_INFO(slog, "ENTRY");
            if (!updateConfig())
            {
                postEvent(EventCameraError{});
            }

            try
            {
                camera.setCaptureTarget("Memory card");
            }
            catch (std::exception& e)
            {
                LOG_ERR(slog, "Error setting capture target: {}", e.what());
                postEvent(EventCameraError{});
            }
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_INFO(slog, "EXIT");
            break;
        case EventCameraError::id:
            retState = transition(&CameraController::stateError);
            break;
        case EventConfigSetShutterSpeed::id:
        case EventConfigSetAperture::id:
        case EventConfigSetISO::id:
        case EventConfigSetCaptureTarget::id:
            if (!setConfig(ev))
                retState = transition(&CameraController::stateError);
            break;
        case EventConfigGetShutterSpeed::id:
        case EventConfigGetAperture::id:
        case EventConfigGetISO::id:
        case EventConfigGetFocalLength::id:
        case EventConfigGetFocusMode::id:
        case EventConfigGetLongExpNR::id:
        case EventConfigGetVibRed::id:
        case EventConfigGetBattery::id:
        case EventConfigGetCaptureTarget::id:
            if (!getConfig(ev))
                retState = transition(&CameraController::stateError);
            break;
        case EventCameraCmdCapture::id:

            retState = transition(&CameraController::stateCapturing);
            break;
        default:
            retState = tran_super(&CameraController::stateSuper);
            break;
    }
    return retState;
}

State CameraController::stateConnectionError(const EventPtr& ev)
{
    auto slog      = log.getChild("stateConnectionError");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            onStateChanged(CCState::CONNECTION_ERROR);
            sEventBroker.post(EventCameraConnectionError{}, TOPIC_CAMERA_EVENT);
            LOG_ERR(slog, "ENTRY");
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_INFO(slog, "EXIT");
            break;
        case EventCameraCmdConnect::id:
            if (connect())
                retState = transition(&CameraController::stateConnected);
            else
                retState = transition(&CameraController::stateConnectionError);

            break;
        default:
            retState = tran_super(&CameraController::stateSuper);
            break;
    }
    return retState;
}

State CameraController::stateError(const EventPtr& ev)
{
    auto slog                           = log.getChild("stateError");
    State retState                      = HANDLED;
    static constexpr int RETRY_DELAY_MS = 2000;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            LOG_ERR(slog, "ENTRY");
            onStateChanged(CCState::ERROR);
            sEventBroker.post(EventCameraError{}, TOPIC_CAMERA_EVENT);
            state_error_recover_event_id = sEventBroker.postDelayed(
                EventCameraCmdRecoverError{}, TOPIC_CAMERA_CMD, RETRY_DELAY_MS);
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_INFO(slog, "EXIT");
            sEventBroker.removeDelayed(state_error_recover_event_id);
            break;
        case EventCameraIgnoreError::id:
            retState = transition(&CameraController::stateConnected);
        case EventCameraCmdDisconnect::id:
            camera.disconnect();
            retState = transition(&CameraController::stateDisconnected);
            break;
        case EventCameraCmdRecoverError::id:
            camera.disconnect();
            sleep_for(milliseconds(500));
            if (connect())
            {
                retState = transition(&CameraController::stateConnected);
            }
            else
            {
                state_error_recover_event_id =
                    sEventBroker.postDelayed(EventCameraCmdRecoverError{},
                                             TOPIC_CAMERA_CMD, RETRY_DELAY_MS);
            }
            break;
        default:
            retState = tran_super(&CameraController::stateSuper);
            break;
    }
    return retState;
}

State CameraController::stateCapturing(const EventPtr& ev)
{
    auto slog      = log.getChild("stateCapturing");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            LOG_INFO(slog, "ENTRY");
            onStateChanged(CCState::CAPTURING);
            postEvent(EventCameraCmdCapture_Internal{});
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_INFO(slog, "EXIT");
            break;
        case EventCameraCmdCapture_Internal::id:
        {
            try
            {
                last_capture_path = camera.capture();
                LOG_INFO(slog, "Capture successfull: {}",
                         last_capture_path.getPath());
                if (do_download)
                {
                    LOG_DEBUG(slog, "Transitioning to download state");
                    retState = transition(&CameraController::stateDownloading);
                }
                else
                {
                    LOG_DEBUG(slog, "Transitioning to connected state");

                    retState = transition(&CameraController::stateConnected);
                    sEventBroker.post(EventCameraCaptureDone{false, ""},
                                      TOPIC_CAMERA_EVENT);
                }
            }
            catch (gphotow::GPhotoError& gpe)
            {
                LOG_ERR(slog, "Camera capture error (GPhoto): {} = {}",
                        gpe.error, gpe.what());
                retState = transition(&CameraController::stateError);
            }
            catch (std::exception& e)
            {
                LOG_ERR(slog, "Camera capture error: {}", e.what());
                retState = transition(&CameraController::stateError);
            }
            break;
        }
        default:
            retState = tran_super(&CameraController::stateSuper);
            break;
    }
    return retState;
}

State CameraController::stateDownloading(const EventPtr& ev)
{
    auto slog      = log.getChild("stateDownloading");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            LOG_INFO(slog, "ENTRY");
            onStateChanged(CCState::DOWNLOADING);
            postEvent(EventCameraCmdDownload_Internal{});
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_INFO(slog, "EXIT");
            break;
        case EventCameraCmdDownload_Internal::id:
        {
            try
            {
                camera.downloadFile(last_capture_path.toCameraFilePath(),
                                    path(download_dir)
                                        .append(last_capture_path.name)
                                        .generic_string());
                sEventBroker.post(
                    EventCameraCaptureDone{
                        true, download_dir + last_capture_path.name},
                    TOPIC_CAMERA_EVENT);
                retState = transition(&CameraController::stateConnected);
            }
            catch (gphotow::GPhotoError& gpe)
            {
                LOG_ERR(slog, "Camera download error (GPhoto): {} = {}",
                        gpe.error, gpe.what());
                retState = transition(&CameraController::stateError);
            }
            catch (std::exception& e)
            {
                LOG_ERR(slog, "Camera download error: {}", e.what());
                retState = transition(&CameraController::stateError);
            }
            break;
        }
        default:
            retState = tran_super(&CameraController::stateSuper);
            break;
    }
    return retState;
}

bool CameraController::updateConfig()
{
    try
    {
        camera_config.shutter_speed = camera.getShutterSpeed();
        camera_config.aperture      = camera.getAperture();
        camera_config.iso           = camera.getISO();

        camera_config.focal_length = camera.getFocalLength();
        camera_config.mode         = camera.getExposureProgram();
        // camera_config.focus_mode = camera.getFocusMode();
        camera_config.long_exposure_nr = camera.getLongExpNR() == "On";
        // camera_config.vr = camera.getVR();
        camera_config.battery = camera.getBatteryPercent();

        sEventBroker.post(EventConfigValueCommon{camera_config},
                          TOPIC_CAMERA_EVENT);
        return true;
    }
    catch (gphotow::GPhotoError& gpe)
    {
        LOG_ERR(log, "updateConfig error (GPhoto): {} = {}", gpe.error,
                gpe.what());
    }
    catch (std::exception& e)
    {
        LOG_ERR(log, "updateConfig error: {}", e.what());
    }
    return false;
}

bool CameraController::connect()
{
    try
    {
        camera.connect();
        return true;
    }
    catch (gphotow::GPhotoError& gpe)
    {
        LOG_ERR(log, "Camera connection error (GPhoto): {} = {}", gpe.error,
                gpe.what());
    }
    catch (std::exception& e)
    {
        LOG_ERR(log, "Camera connection error: {}", e.what());
    }
    return false;
}

bool CameraController::setConfig(const EventPtr& ev)
{
    try
    {
        switch (ev->getID())
        {
            case EventConfigSetShutterSpeed::id:
            {
                auto set_ev =
                    dynamic_pointer_cast<const EventConfigSetShutterSpeed>(ev);
                camera.setShutterSpeed(set_ev->shutter_speed);
                sEventBroker.post(
                    EventConfigValueShutterSpeed{camera.getShutterSpeed()},
                    TOPIC_CAMERA_CONFIG);
                break;
            }
            case EventConfigSetAperture::id:
            {
                auto set_ev =
                    dynamic_pointer_cast<const EventConfigSetAperture>(ev);
                camera.setAperture(set_ev->aperture);
                sEventBroker.post(
                    EventConfigValueAperture{camera.getAperture()},
                    TOPIC_CAMERA_CONFIG);
                break;
            }
            case EventConfigSetISO::id:
            {
                auto set_ev = dynamic_pointer_cast<const EventConfigSetISO>(ev);
                camera.setISO(set_ev->iso);
                sEventBroker.post(EventConfigValueISO{camera.getISO()},
                                  TOPIC_CAMERA_CONFIG);
                break;
            }
            case EventConfigSetCaptureTarget::id:
            {
                auto set_ev =
                    dynamic_pointer_cast<const EventConfigSetCaptureTarget>(ev);
                camera.setCaptureTarget(set_ev->target);
                sEventBroker.post(
                    EventConfigValueCaptureTarget{camera.getCaptureTarget()},
                    TOPIC_CAMERA_CONFIG);
                break;
            }
            default:
                LOG_ERR(log, "Unknow config setter event: {}", ev->name());
                break;
        }
        return true;
    }
    catch (gphotow::GPhotoError& gpe)
    {
        LOG_ERR(log, "Camera get config error ({}) (GPhoto): {} = {}",
                ev->name(), gpe.error, gpe.what());
    }
    catch (std::exception& e)
    {
        LOG_ERR(log, "Camera get config error ({}): {}", ev->name(), e.what());
    }
    return false;
}

bool CameraController::getConfig(const EventPtr& ev)
{
    try
    {
        switch (ev->getID())
        {
            case EventConfigGetShutterSpeed::id:
                sEventBroker.post(
                    EventConfigValueShutterSpeed{camera.getShutterSpeed()},
                    TOPIC_CAMERA_CONFIG);
                break;
            case EventConfigGetAperture::id:
                sEventBroker.post(
                    EventConfigValueAperture{camera.getAperture()},
                    TOPIC_CAMERA_CONFIG);
                break;
            case EventConfigGetISO::id:
                sEventBroker.post(EventConfigValueISO{camera.getISO()},
                                  TOPIC_CAMERA_CONFIG);
                break;
            case EventConfigGetFocalLength::id:
                sEventBroker.post(
                    EventConfigValueFocalLength{camera.getFocalLength()},
                    TOPIC_CAMERA_CONFIG);
                break;
            case EventConfigGetFocusMode::id:
                // sEventBroker.post(
                //     EventConfigValueFocusMode{camera.getF()},
                //     TOPIC_CAMERA_CONFIG);
                break;
            case EventConfigGetLongExpNR::id:
                sEventBroker.post(
                    EventConfigValueLongExpNR{camera.getLongExpNR() == "On"},
                    TOPIC_CAMERA_CONFIG);
                break;
            case EventConfigGetVibRed::id:
                // sEventBroker.post(
                //     EventConfigValueVibRed{camera.getFocalLength()},
                //     TOPIC_CAMERA_CONFIG);
                break;
            case EventConfigGetBattery::id:
                sEventBroker.post(
                    EventConfigValueBattery{camera.getBatteryPercent()},
                    TOPIC_CAMERA_CONFIG);
                break;
            case EventConfigGetCaptureTarget::id:
                sEventBroker.post(
                    EventConfigValueCaptureTarget{camera.getCaptureTarget()},
                    TOPIC_CAMERA_CONFIG);
                break;
            default:
                LOG_ERR(log, "Unknow config getter event: {}", ev->name());
                break;
        }
        return true;
    }
    catch (gphotow::GPhotoError& gpe)
    {
        LOG_ERR(log, "Camera get config error ({}) (GPhoto): {} = {}",
                ev->name(), gpe.error, gpe.what());
    }
    catch (std::exception& e)
    {
        LOG_ERR(log, "Camera get config error ({}): {}", ev->name(), e.what());
    }
    return false;
}

void CameraController::onStateChanged(CCState state)
{
    this->state = state;
    sEventBroker.post(EventCameraControllerState{static_cast<uint8_t>(state)},
                      TOPIC_CAMERA_EVENT);
}
