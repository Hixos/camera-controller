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
    auto slog      = log.getChild("Super");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            LOG_STATE(slog, "ENTRY");
            break;
        case EventSMInit::id:
            retState = transition(&CameraController::stateDisconnected);
            break;
        case EventSMExit::id:
            LOG_STATE(slog, "EXIT");
            break;
        case EventCameraCmdDownload::id:
        {
            auto d_ev = dynamic_pointer_cast<const EventCameraCmdDownload>(ev);
            do_download = d_ev->download;
            LOG_INFO(slog, "Camera download enabled={}", do_download);
            getState();
            break;
        }
        case EventCameraCmdLowLatency::id:
        {
            auto d_ev =
                dynamic_pointer_cast<const EventCameraCmdLowLatency>(ev);
            low_latency = d_ev->low_latency;
            break;
        }
        case EventGetCameraControllerState::id:
        {
            getState();
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
    auto slog      = log.getChild("Disconn");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            onStateChanged(CCState::DISCONNECTED);
            LOG_STATE(slog, "ENTRY");
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_STATE(slog, "EXIT");
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
    auto slog      = log.getChild("Conn");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            sEventBroker.post(EventCameraConnected{}, TOPIC_CAMERA_EVENT);

            LOG_STATE(slog, "ENTRY");

            try
            {
                camera.setCaptureTarget("Memory card");
            }
            catch (std::exception& e)
            {
                LOG_ERR(slog, "Error setting capture target: {}", e.what());
                checkConnection();
            }
            onCameraConnected(true);
            break;
        case EventSMInit::id:
            retState = transition(&CameraController::stateReady);
            break;
        case EventSMExit::id:
            LOG_STATE(slog, "EXIT");
            onCameraConnected(false);
            break;
        case EventCameraError::id:
            retState = transition(&CameraController::stateError);
            break;
        default:
            retState = tran_super(&CameraController::stateSuper);
            break;
    }
    return retState;
}

State CameraController::stateReady(const EventPtr& ev)
{
    auto slog      = log.getChild("Ready");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            LOG_STATE(slog, "ENTRY");
            onStateChanged(CCState::READY);
            processDeferred();
            
            sEventBroker.post(EventCameraReady{}, TOPIC_CAMERA_EVENT);

            if (!low_latency)
            {
                if (!getAllConfig())
                {
                    checkConnection();
                }
            }
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            sEventBroker.post(EventCameraBusyOrError{}, TOPIC_CAMERA_EVENT);
            LOG_STATE(slog, "EXIT");
            break;
        case EventConfigGetAll::id:
            if (!getAllConfig())
                checkConnection();
            break;
        case EventCameraCmdCapture::id:
            retState = transition(&CameraController::stateCapturing);
            break;
        default:
        {
            retState = handleConfigGetSet(ev);
            break;
        }
    }
    return retState;
}


State CameraController::stateConnectionError(const EventPtr& ev)
{
    auto slog                           = log.getChild("ConnErr");
    State retState                      = HANDLED;
    static constexpr int RETRY_DELAY_MS = 5000;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            onStateChanged(CCState::CONNECTION_ERROR);
            sEventBroker.post(EventCameraConnectionError{}, TOPIC_CAMERA_EVENT);
            LOG_STATE(slog, "ENTRY");
            state_error_recover_event_id = sEventBroker.postDelayed(
                EventCameraCmdRecoverError{}, TOPIC_CAMERA_CMD, RETRY_DELAY_MS);
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_STATE(slog, "EXIT");
            break;
        case EventCameraCmdConnect::id:
            if (connect())
                retState = transition(&CameraController::stateConnected);
            else
                retState = transition(&CameraController::stateConnectionError);

            break;
        case EventCameraCmdRecoverError::id:
            if (connect())
            {
                retState = transition(&CameraController::stateConnected);
            }
            else
            {
                retState = transition(&CameraController::stateConnectionError);
            }
            break;
        default:
            retState = tran_super(&CameraController::stateSuper);
            break;
    }
    return retState;
}

State CameraController::stateError(const EventPtr& ev)
{
    auto slog                           = log.getChild("Error");
    State retState                      = HANDLED;
    static constexpr int RETRY_DELAY_MS = 5000;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            LOG_STATE(slog, "ENTRY");
            onStateChanged(CCState::ERROR);
            sEventBroker.post(EventCameraError{}, TOPIC_CAMERA_EVENT);
            state_error_recover_event_id = sEventBroker.postDelayed(
                EventCameraCmdRecoverError{}, TOPIC_CAMERA_CMD, RETRY_DELAY_MS);
            processDeferred();
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_STATE(slog, "EXIT");
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
                retState = transition(&CameraController::stateError);
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
    auto slog      = log.getChild("Capture");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            LOG_STATE(slog, "ENTRY");
            onStateChanged(CCState::CAPTURING);
            postEvent(EventCameraCmdCapture_Internal{});
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_STATE(slog, "EXIT");
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
                    retState = transition(&CameraController::stateDownloading);
                }
                else
                {
                    retState = transition(&CameraController::stateReady);
                    sEventBroker.post(EventCameraCaptureDone{false, "", ""},
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
            if(!deferSetters(ev))
                retState = tran_super(&CameraController::stateConnected);
            break;
    }
    return retState;
}

State CameraController::stateDownloading(const EventPtr& ev)
{
    auto slog      = log.getChild("Download");
    State retState = HANDLED;
    switch (ev->getID())
    {
        case EventSMEntry::id:
            LOG_STATE(slog, "ENTRY");
            onStateChanged(CCState::DOWNLOADING);
            postEvent(EventCameraCmdDownload_Internal{});
            break;
        case EventSMInit::id:
            break;
        case EventSMExit::id:
            LOG_STATE(slog, "EXIT");
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
                        true, download_dir, last_capture_path.name},
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
            if(!deferSetters(ev))
                retState = tran_super(&CameraController::stateConnected);
            break;
    }
    return retState;
}

State CameraController::handleConfigGetSet(const EventPtr& ev)
{
    ConfigEventHandleResult s = getConfig(ev);
    if (s == ConfigEventHandleResult::UNHANDLED)
        s = setConfig(ev);

    switch (s)
    {
        case ConfigEventHandleResult::HANDLED:
            return State::HANDLED;
        case ConfigEventHandleResult::ERROR:
            checkConnection();
            return State::HANDLED;
        default:
            return tran_super(&CameraController::stateConnected);
    }
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

bool CameraController::deferSetters(const EventPtr& ev)
{
    if (config_setters.count(ev->getID()) > 0)
    {
        defer(ev);
        return true;
    }
    return false;
}

CameraController::ConfigEventHandleResult CameraController::setConfig(
    const EventPtr& ev)
{
    try
    {
        if (config_setters.count(ev->getID()) > 0)
        {
            config_setters.at(ev->getID())(*this, ev);
            return ConfigEventHandleResult::HANDLED;
        }
        return ConfigEventHandleResult::UNHANDLED;
    }
    catch (gphotow::GPhotoError& gpe)
    {
        LOG_ERR(log, "Camera set config error ({}) (GPhoto): {} = {}",
                ev->name(), gpe.error, gpe.what());
    }
    catch (std::exception& e)
    {
        LOG_ERR(log, "Camera set config error ({}): {}", ev->name(), e.what());
    }
    return ConfigEventHandleResult::ERROR;
}

CameraController::ConfigEventHandleResult CameraController::getConfig(
    const EventPtr& ev)
{
    try
    {
        if (config_getters.count(ev->getID()) > 0)
        {
            config_getters.at(ev->getID())(*this);
            return ConfigEventHandleResult::HANDLED;
        }
        return ConfigEventHandleResult::UNHANDLED;
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
    return ConfigEventHandleResult::ERROR;
}

bool CameraController::getAllConfig()
{
    try
    {
        for (auto it = config_getters.begin(); it != config_getters.end(); it++)
        {
            it->second(*this);
        }
        getState();
        return true;
    }
    catch (gphotow::GPhotoError& gpe)
    {
        LOG_ERR(log, "Camera get all config error (GPhoto): {} = {}", gpe.error,
                gpe.what());
    }
    catch (std::exception& e)
    {
        LOG_ERR(log, "Camera get all config error: {}", e.what());
    }
    return false;
}

void CameraController::onStateChanged(CCState state)
{
    this->state = state;
    getState();
}

void CameraController::onCameraConnected(bool connected)
{
    this->camera_connected = connected;
    getState();
}

void CameraController::getState()
{
    EventCameraControllerState e;
    e.camera_connected = camera_connected;
    e.state            = state_names.at(state);
    e.download_enabled = do_download;

    sEventBroker.post(std::move(e), TOPIC_CAMERA_CONFIG);
}

void CameraController::checkConnection()
{
    try
    {
        auto ss = camera.getShutterSpeed().shutter_speed;
        camera.setShutterSpeed(1000000);
        camera.setShutterSpeed(ss);
        return;
    }
    catch (gphotow::GPhotoError& gpe)
    {
        LOG_ERR(log, "Error communicating with camera (GPhoto): {} = {}",
                gpe.error, gpe.what());
    }
    catch (std::exception& e)
    {
        LOG_ERR(log, "Error communicating with camera: {}", e.what());
    }
    postEvent(EventCameraError{});
}