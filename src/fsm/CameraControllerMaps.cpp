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

#include <memory>

#include "CameraController.h"
#include "EventBroker.h"
#include "camera/CameraWidget.h"

using namespace gphotow;
using std::dynamic_pointer_cast;

const map<uint16_t, function<void(CameraController&)>>
    CameraController::config_getters{
        {EventConfigGetShutterSpeed::id,
         [](CameraController& cc) {
             auto ss = cc.camera.getShutterSpeed();

             sEventBroker.post(
                 EventConfigValueShutterSpeed{ss.shutter_speed, ss.bulb},
                 TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetChoicesShutterSpeed::id,
         [](CameraController& cc) {
             sEventBroker.post(
                 EventConfigChoicesShutterSpeed{
                     cc.camera.getShutterSpeedChoices(false)},
                 TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetAperture::id,
         [](CameraController& cc) {
             sEventBroker.post(
                 EventConfigValueAperture{cc.camera.getAperture()},
                 TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetChoicesAperture::id,
         [](CameraController& cc) {
             sEventBroker.post(
                 EventConfigChoicesAperture{cc.camera.getApertureChoices()},
                 TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetISO::id,
         [](CameraController& cc) {
             sEventBroker.post(EventConfigValueISO{cc.camera.getISO()},
                               TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetChoicesISO::id,
         [](CameraController& cc) {
             sEventBroker.post(EventConfigChoicesISO{cc.camera.getIsoChoices()},
                               TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetFocalLength::id,
         [](CameraController& cc) {
             sEventBroker.post(
                 EventConfigValueFocalLength{cc.camera.getFocalLength()},
                 TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetFocusMode::id,
         [](CameraController& cc) {
             sEventBroker.post(
                 EventConfigValueFocusMode{cc.camera.getFocusMode()},
                 TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetLongExpNR::id,
         [](CameraController& cc) {
             sEventBroker.post(
                 EventConfigValueLongExpNR{cc.camera.getLongExpNR()},
                 TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetAutoISO::id,
         [](CameraController& cc) {
             sEventBroker.post(EventConfigValueAutoISO{cc.camera.getAutoISO()},
                               TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetBattery::id,
         [](CameraController& cc) {
             sEventBroker.post(
                 EventConfigValueBattery{cc.camera.getBatteryPercent()},
                 TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetCaptureTarget::id,
         [](CameraController& cc) {
             sEventBroker.post(
                 EventConfigValueCaptureTarget{cc.camera.getCaptureTarget()},
                 TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetExposureProgram::id,
         [](CameraController& cc) {
             sEventBroker.post(
                 EventConfigValueExposureProgram{cc.camera.getExposureProgram()},
                 TOPIC_CAMERA_CONFIG);
         }},
        {EventConfigGetLightMeter::id, [](CameraController& cc) {
             float val                      = cc.camera.getLightMeter();
             CameraWidgetRange::Range range = cc.camera.getLightMeterRange();

             sEventBroker.post(
                 EventConfigValueLightMeter(val, range.min, range.max),
                 TOPIC_CAMERA_CONFIG);
         }}};

const map<uint16_t, function<void(CameraController&, const EventPtr&)>>
    CameraController::config_setters{
        {EventConfigSetShutterSpeed::id,
         [](CameraController& cc, const EventPtr& ev) {
             auto set_ev =
                 dynamic_pointer_cast<const EventConfigSetShutterSpeed>(ev);
             cc.camera.setShutterSpeed(set_ev->shutter_speed);
             config_getters.at(EventConfigGetShutterSpeed::id)(cc);
         }},
        {EventConfigSetAperture::id,
         [](CameraController& cc, const EventPtr& ev) {
             auto set_ev =
                 dynamic_pointer_cast<const EventConfigSetAperture>(ev);
             cc.camera.setAperture(set_ev->aperture);
             config_getters.at(EventConfigGetAperture::id)(cc);
         }},
        {EventConfigSetISO::id,
         [](CameraController& cc, const EventPtr& ev) {
             auto set_ev = dynamic_pointer_cast<const EventConfigSetISO>(ev);
             cc.camera.setISO(set_ev->iso);
             config_getters.at(EventConfigGetISO::id)(cc);
         }},
        {EventConfigSetCaptureTarget::id,
         [](CameraController& cc, const EventPtr& ev) {
             auto set_ev =
                 dynamic_pointer_cast<const EventConfigSetCaptureTarget>(ev);
             cc.camera.setCaptureTarget(set_ev->target);
             config_getters.at(EventConfigGetCaptureTarget::id)(cc);
         }},
        {EventConfigNextFocusMode::id,
         [](CameraController& cc, const EventPtr& ev) {
             cc.camera.nextFocusMode();
             config_getters.at(EventConfigGetFocusMode::id)(cc);
         }},
        {EventConfigSetLongExpNR::id,
         [](CameraController& cc, const EventPtr& ev) {
             auto set_ev =
                 dynamic_pointer_cast<const EventConfigSetLongExpNR>(ev);
             cc.camera.setLongExpNR(set_ev->long_exp_nr);
             config_getters.at(EventConfigGetLongExpNR::id)(cc);
         }},
        {EventConfigSetAutoISO::id,
         [](CameraController& cc, const EventPtr& ev) {
             auto set_ev =
                 dynamic_pointer_cast<const EventConfigSetAutoISO>(ev);
             cc.camera.setAutoISO(set_ev->auto_iso);
             config_getters.at(EventConfigGetAutoISO::id)(cc);
         }}

    };

const map<CameraController::CCState, string> CameraController::state_names{
    {CCState::DISCONNECTED, "Disconnected"},
    {CCState::READY, "Ready"},
    {CCState::CONNECTION_ERROR, "Connection Error"},
    {CCState::ERROR, "Error"},
    {CCState::CAPTURING, "Capturing"},
    {CCState::DOWNLOADING, "Downloading"}
};