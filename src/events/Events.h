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

/*
 ******************************************************************************
 *                  THIS FILE IS AUTOGENERATED. DO NOT EDIT.                  *
 ******************************************************************************
 */

// Autogen date:    2022-07-31 18:06:30.300318

#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

#include "EventBase.h"

using std::string;
using std::vector;

enum Topics : uint8_t
{
    TOPIC_CAMERA_CONFIG,
    TOPIC_CAMERA_CMD,
    TOPIC_CAMERA_EVENT,
    TOPIC_REMOTE_CMD,
    TOPIC_MODE_CONTROLLER,
    TOPIC_MODE_FSM,
    TOPIC_MODE_STATE,
    TOPIC_HEARTBEAT
};

string getTopicName(uint8_t topic);
uint8_t getTopicID(string topic_str);
EventPtr jsonToEvent(const nlohmann::json& j);

struct EventHeartBeat : public Event
{
    static constexpr uint16_t id = 10;

    EventHeartBeat();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventHeartBeat);
};

struct EventCmdRestart : public Event
{
    static constexpr uint16_t id = 11;

    EventCmdRestart();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCmdRestart);
};

struct EventCmdReboot : public Event
{
    static constexpr uint16_t id = 12;

    EventCmdReboot();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCmdReboot);
};

struct EventCmdShutdown : public Event
{
    static constexpr uint16_t id = 13;

    EventCmdShutdown();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCmdShutdown);
};

struct EventCameraCmdConnect : public Event
{
    static constexpr uint16_t id = 14;

    EventCameraCmdConnect();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdConnect);
};

struct EventCameraCmdDisconnect : public Event
{
    static constexpr uint16_t id = 15;

    EventCameraCmdDisconnect();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdDisconnect);
};

struct EventCameraCmdRecoverError : public Event
{
    static constexpr uint16_t id = 16;

    EventCameraCmdRecoverError();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdRecoverError);
};

struct EventCameraCaptureStarted : public Event
{
    static constexpr uint16_t id = 17;

    EventCameraCaptureStarted();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCaptureStarted);
};

struct EventCameraCmdCapture : public Event
{
    static constexpr uint16_t id = 18;

    EventCameraCmdCapture();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdCapture);
};

struct EventCameraCmdCapture_Internal : public Event
{
    static constexpr uint16_t id = 19;

    EventCameraCmdCapture_Internal();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdCapture_Internal);
};

struct EventCameraCmdDownload : public Event
{
    static constexpr uint16_t id = 20;

    EventCameraCmdDownload() : Event(id){};
    EventCameraCmdDownload(bool download);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    bool download;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventCameraCmdDownload, download);
};

struct EventCameraCmdDownload_Internal : public Event
{
    static constexpr uint16_t id = 21;

    EventCameraCmdDownload_Internal();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdDownload_Internal);
};

struct EventCameraConnected : public Event
{
    static constexpr uint16_t id = 22;

    EventCameraConnected();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraConnected);
};

struct EventCameraReady : public Event
{
    static constexpr uint16_t id = 23;

    EventCameraReady();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraReady);
};

struct EventCameraBusyOrError : public Event
{
    static constexpr uint16_t id = 24;

    EventCameraBusyOrError();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraBusyOrError);
};

struct EventCameraDisconnected : public Event
{
    static constexpr uint16_t id = 25;

    EventCameraDisconnected();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraDisconnected);
};

struct EventCameraConnectionError : public Event
{
    static constexpr uint16_t id = 26;

    EventCameraConnectionError();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraConnectionError);
};

struct EventCameraError : public Event
{
    static constexpr uint16_t id = 27;

    EventCameraError();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraError);
};

struct EventCameraIgnoreError : public Event
{
    static constexpr uint16_t id = 28;

    EventCameraIgnoreError();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraIgnoreError);
};

struct EventCameraCmdLowLatency : public Event
{
    static constexpr uint16_t id = 29;

    EventCameraCmdLowLatency() : Event(id){};
    EventCameraCmdLowLatency(bool low_latency);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    bool low_latency;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventCameraCmdLowLatency, low_latency);
};

struct EventCameraCaptureDone : public Event
{
    static constexpr uint16_t id = 30;

    EventCameraCaptureDone() : Event(id){};
    EventCameraCaptureDone(bool downloaded, string download_dir, string file);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    bool downloaded;
    string download_dir;
    string file;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventCameraCaptureDone, downloaded,
                                       download_dir, file);
};

struct EventGetCameraControllerState : public Event
{
    static constexpr uint16_t id = 31;

    EventGetCameraControllerState();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventGetCameraControllerState);
};

struct EventCameraControllerState : public Event
{
    static constexpr uint16_t id = 32;

    EventCameraControllerState() : Event(id){};
    EventCameraControllerState(string state, bool camera_connected,
                               bool download_enabled);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    string state;
    bool camera_connected;
    bool download_enabled;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventCameraControllerState, state,
                                       camera_connected, download_enabled);
};

struct EventConfigGetShutterSpeed : public Event
{
    static constexpr uint16_t id = 33;

    EventConfigGetShutterSpeed();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetShutterSpeed);
};

struct EventConfigGetChoicesShutterSpeed : public Event
{
    static constexpr uint16_t id = 34;

    EventConfigGetChoicesShutterSpeed();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(
        EventConfigGetChoicesShutterSpeed);
};

struct EventConfigSetShutterSpeed : public Event
{
    static constexpr uint16_t id = 35;

    EventConfigSetShutterSpeed() : Event(id){};
    EventConfigSetShutterSpeed(int32_t shutter_speed);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t shutter_speed;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigSetShutterSpeed,
                                       shutter_speed);
};

struct EventConfigValueShutterSpeed : public Event
{
    static constexpr uint16_t id = 36;

    EventConfigValueShutterSpeed() : Event(id){};
    EventConfigValueShutterSpeed(int32_t shutter_speed, bool bulb);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t shutter_speed;
    bool bulb;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueShutterSpeed,
                                       shutter_speed, bulb);
};

struct EventConfigChoicesShutterSpeed : public Event
{
    static constexpr uint16_t id = 37;

    EventConfigChoicesShutterSpeed() : Event(id){};
    EventConfigChoicesShutterSpeed(vector<int32_t> shutter_speed_choices);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    vector<int32_t> shutter_speed_choices;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigChoicesShutterSpeed,
                                       shutter_speed_choices);
};

struct EventConfigGetAperture : public Event
{
    static constexpr uint16_t id = 38;

    EventConfigGetAperture();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetAperture);
};

struct EventConfigGetChoicesAperture : public Event
{
    static constexpr uint16_t id = 39;

    EventConfigGetChoicesAperture();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetChoicesAperture);
};

struct EventConfigSetAperture : public Event
{
    static constexpr uint16_t id = 40;

    EventConfigSetAperture() : Event(id){};
    EventConfigSetAperture(int32_t aperture);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t aperture;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigSetAperture, aperture);
};

struct EventConfigValueAperture : public Event
{
    static constexpr uint16_t id = 41;

    EventConfigValueAperture() : Event(id){};
    EventConfigValueAperture(int32_t aperture);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t aperture;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueAperture, aperture);
};

struct EventConfigChoicesAperture : public Event
{
    static constexpr uint16_t id = 42;

    EventConfigChoicesAperture() : Event(id){};
    EventConfigChoicesAperture(vector<int32_t> aperture_choices);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    vector<int32_t> aperture_choices;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigChoicesAperture,
                                       aperture_choices);
};

struct EventConfigGetISO : public Event
{
    static constexpr uint16_t id = 43;

    EventConfigGetISO();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetISO);
};

struct EventConfigGetChoicesISO : public Event
{
    static constexpr uint16_t id = 44;

    EventConfigGetChoicesISO();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetChoicesISO);
};

struct EventConfigSetISO : public Event
{
    static constexpr uint16_t id = 45;

    EventConfigSetISO() : Event(id){};
    EventConfigSetISO(int32_t iso);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t iso;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigSetISO, iso);
};

struct EventConfigValueISO : public Event
{
    static constexpr uint16_t id = 46;

    EventConfigValueISO() : Event(id){};
    EventConfigValueISO(int32_t iso);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t iso;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueISO, iso);
};

struct EventConfigChoicesISO : public Event
{
    static constexpr uint16_t id = 47;

    EventConfigChoicesISO() : Event(id){};
    EventConfigChoicesISO(vector<int32_t> iso_choices);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    vector<int32_t> iso_choices;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigChoicesISO, iso_choices);
};

struct EventConfigGetBattery : public Event
{
    static constexpr uint16_t id = 48;

    EventConfigGetBattery();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetBattery);
};

struct EventConfigValueBattery : public Event
{
    static constexpr uint16_t id = 49;

    EventConfigValueBattery() : Event(id){};
    EventConfigValueBattery(int32_t battery);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t battery;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueBattery, battery);
};

struct EventConfigGetFocalLength : public Event
{
    static constexpr uint16_t id = 50;

    EventConfigGetFocalLength();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetFocalLength);
};

struct EventConfigValueFocalLength : public Event
{
    static constexpr uint16_t id = 51;

    EventConfigValueFocalLength() : Event(id){};
    EventConfigValueFocalLength(int32_t focal_length);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t focal_length;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueFocalLength,
                                       focal_length);
};

struct EventConfigGetFocusMode : public Event
{
    static constexpr uint16_t id = 52;

    EventConfigGetFocusMode();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetFocusMode);
};

struct EventConfigNextFocusMode : public Event
{
    static constexpr uint16_t id = 53;

    EventConfigNextFocusMode();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigNextFocusMode);
};

struct EventConfigValueFocusMode : public Event
{
    static constexpr uint16_t id = 54;

    EventConfigValueFocusMode() : Event(id){};
    EventConfigValueFocusMode(string focus_mode);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    string focus_mode;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueFocusMode, focus_mode);
};

struct EventConfigGetLongExpNR : public Event
{
    static constexpr uint16_t id = 55;

    EventConfigGetLongExpNR();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetLongExpNR);
};

struct EventConfigSetLongExpNR : public Event
{
    static constexpr uint16_t id = 56;

    EventConfigSetLongExpNR() : Event(id){};
    EventConfigSetLongExpNR(bool long_exp_nr);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    bool long_exp_nr;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigSetLongExpNR, long_exp_nr);
};

struct EventConfigValueLongExpNR : public Event
{
    static constexpr uint16_t id = 57;

    EventConfigValueLongExpNR() : Event(id){};
    EventConfigValueLongExpNR(bool long_exp_nr);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    bool long_exp_nr;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueLongExpNR, long_exp_nr);
};

struct EventConfigGetVibRed : public Event
{
    static constexpr uint16_t id = 58;

    EventConfigGetVibRed();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetVibRed);
};

struct EventConfigSetVibRed : public Event
{
    static constexpr uint16_t id = 59;

    EventConfigSetVibRed() : Event(id){};
    EventConfigSetVibRed(bool vr);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    bool vr;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigSetVibRed, vr);
};

struct EventConfigValueVibRed : public Event
{
    static constexpr uint16_t id = 60;

    EventConfigValueVibRed() : Event(id){};
    EventConfigValueVibRed(bool vr);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    bool vr;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueVibRed, vr);
};

struct EventConfigGetCaptureTarget : public Event
{
    static constexpr uint16_t id = 61;

    EventConfigGetCaptureTarget();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetCaptureTarget);
};

struct EventConfigSetCaptureTarget : public Event
{
    static constexpr uint16_t id = 62;

    EventConfigSetCaptureTarget() : Event(id){};
    EventConfigSetCaptureTarget(string target);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    string target;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigSetCaptureTarget, target);
};

struct EventConfigValueCaptureTarget : public Event
{
    static constexpr uint16_t id = 63;

    EventConfigValueCaptureTarget() : Event(id){};
    EventConfigValueCaptureTarget(string target);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    string target;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueCaptureTarget, target);
};

struct EventConfigGetExposureProgram : public Event
{
    static constexpr uint16_t id = 64;

    EventConfigGetExposureProgram();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetExposureProgram);
};

struct EventConfigValueExposureProgram : public Event
{
    static constexpr uint16_t id = 65;

    EventConfigValueExposureProgram() : Event(id){};
    EventConfigValueExposureProgram(string exposure_program);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    string exposure_program;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueExposureProgram,
                                       exposure_program);
};

struct EventConfigGetLightMeter : public Event
{
    static constexpr uint16_t id = 66;

    EventConfigGetLightMeter();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetLightMeter);
};

struct EventConfigValueLightMeter : public Event
{
    static constexpr uint16_t id = 67;

    EventConfigValueLightMeter() : Event(id){};
    EventConfigValueLightMeter(float light_meter, float min, float max);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    float light_meter;
    float min;
    float max;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueLightMeter, light_meter,
                                       min, max);
};

struct EventConfigGetAutoISO : public Event
{
    static constexpr uint16_t id = 68;

    EventConfigGetAutoISO();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetAutoISO);
};

struct EventConfigSetAutoISO : public Event
{
    static constexpr uint16_t id = 69;

    EventConfigSetAutoISO() : Event(id){};
    EventConfigSetAutoISO(bool auto_iso);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    bool auto_iso;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigSetAutoISO, auto_iso);
};

struct EventConfigValueAutoISO : public Event
{
    static constexpr uint16_t id = 70;

    EventConfigValueAutoISO() : Event(id){};
    EventConfigValueAutoISO(bool auto_iso);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    bool auto_iso;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueAutoISO, auto_iso);
};

struct EventConfigGetAll : public Event
{
    static constexpr uint16_t id = 71;

    EventConfigGetAll();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetAll);
};

struct EventGetCurrentMode : public Event
{
    static constexpr uint16_t id = 72;

    EventGetCurrentMode();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventGetCurrentMode);
};

struct EventValueCurrentMode : public Event
{
    static constexpr uint16_t id = 73;

    EventValueCurrentMode() : Event(id){};
    EventValueCurrentMode(string mode);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    string mode;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventValueCurrentMode, mode);
};

struct EventModeStopped : public Event
{
    static constexpr uint16_t id = 74;

    EventModeStopped();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventModeStopped);
};

struct EventModeStop : public Event
{
    static constexpr uint16_t id = 75;

    EventModeStop();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventModeStop);
};

struct EventModeIntervalometer : public Event
{
    static constexpr uint16_t id = 76;

    EventModeIntervalometer() : Event(id){};
    EventModeIntervalometer(int32_t intervalms, int32_t total_captures);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t intervalms;
    int32_t total_captures;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventModeIntervalometer, intervalms,
                                       total_captures);
};

struct EventIntervalometerStart : public Event
{
    static constexpr uint16_t id = 77;

    EventIntervalometerStart() : Event(id){};
    EventIntervalometerStart(int32_t intervalms, int32_t total_captures);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t intervalms;
    int32_t total_captures;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventIntervalometerStart, intervalms,
                                       total_captures);
};

struct EventIntervalometerDeadlineExpired : public Event
{
    static constexpr uint16_t id = 78;

    EventIntervalometerDeadlineExpired();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(
        EventIntervalometerDeadlineExpired);
};

struct EventIntervalometerState : public Event
{
    static constexpr uint16_t id = 79;

    EventIntervalometerState() : Event(id){};
    EventIntervalometerState(string state, int32_t intervalms,
                             int32_t num_captures, int32_t total_captures);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    string state;
    int32_t intervalms;
    int32_t num_captures;
    int32_t total_captures;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventIntervalometerState, state,
                                       intervalms, num_captures,
                                       total_captures);
};

struct EventEnableEventPassThrough : public Event
{
    static constexpr uint16_t id = 80;

    EventEnableEventPassThrough();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventEnableEventPassThrough);
};

struct EventDisableEventPassThrough : public Event
{
    static constexpr uint16_t id = 81;

    EventDisableEventPassThrough();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventDisableEventPassThrough);
};
