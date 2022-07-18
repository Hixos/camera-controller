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

// Autogen date:    2022-07-19 01:20:56.724005

#include <cassert>
#include <cstdint>
#include <string>
#include <type_traits>

#include "EventBase.h"

enum Topics : uint8_t
{
    TOPIC_CAMERA_CONFIG,
    TOPIC_CAMERA_CMD,
    TOPIC_CAMERA_EVENT
};

string getTopicName(uint8_t topic);
uint8_t getTopicID(string topic_str);
EventPtr jsonToEvent(const nlohmann::json& j);

struct EventCameraCmdConnect : public Event
{
    static constexpr uint16_t id = 10;

    EventCameraCmdConnect();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdConnect);
};

struct EventCameraCmdDisconnect : public Event
{
    static constexpr uint16_t id = 11;

    EventCameraCmdDisconnect();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdDisconnect);
};

struct EventCameraCmdRecoverError : public Event
{
    static constexpr uint16_t id = 12;

    EventCameraCmdRecoverError();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdRecoverError);
};

struct EventCameraCmdCapture : public Event
{
    static constexpr uint16_t id = 13;

    EventCameraCmdCapture();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdCapture);
};

struct EventCameraCmdCapture_Internal : public Event
{
    static constexpr uint16_t id = 14;

    EventCameraCmdCapture_Internal();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdCapture_Internal);
};

struct EventCameraCmdDownload : public Event
{
    static constexpr uint16_t id = 15;

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
    static constexpr uint16_t id = 16;

    EventCameraCmdDownload_Internal();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraCmdDownload_Internal);
};

struct EventCameraConnected : public Event
{
    static constexpr uint16_t id = 17;

    EventCameraConnected();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraConnected);
};

struct EventCameraReady : public Event
{
    static constexpr uint16_t id = 18;

    EventCameraReady();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraReady);
};

struct EventCameraDisconnected : public Event
{
    static constexpr uint16_t id = 19;

    EventCameraDisconnected();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraDisconnected);
};

struct EventCameraConnectionError : public Event
{
    static constexpr uint16_t id = 20;

    EventCameraConnectionError();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraConnectionError);
};

struct EventCameraError : public Event
{
    static constexpr uint16_t id = 21;

    EventCameraError();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraError);
};

struct EventCameraIgnoreError : public Event
{
    static constexpr uint16_t id = 22;

    EventCameraIgnoreError();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventCameraIgnoreError);
};

struct EventCameraCmdLowLatency : public Event
{
    static constexpr uint16_t id = 23;

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
    static constexpr uint16_t id = 24;

    EventCameraCaptureDone() : Event(id){};
    EventCameraCaptureDone(bool downloaded, string file);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    bool downloaded;
    string file;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventCameraCaptureDone, downloaded,
                                       file);
};

struct EventCameraControllerState : public Event
{
    static constexpr uint16_t id = 25;

    EventCameraControllerState() : Event(id){};
    EventCameraControllerState(uint8_t state);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    uint8_t state;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventCameraControllerState, state);
};

struct EventConfigGetShutterSpeed : public Event
{
    static constexpr uint16_t id = 26;

    EventConfigGetShutterSpeed();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetShutterSpeed);
};

struct EventConfigSetShutterSpeed : public Event
{
    static constexpr uint16_t id = 27;

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
    static constexpr uint16_t id = 28;

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

struct EventConfigGetAperture : public Event
{
    static constexpr uint16_t id = 29;

    EventConfigGetAperture();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetAperture);
};

struct EventConfigSetAperture : public Event
{
    static constexpr uint16_t id = 30;

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
    static constexpr uint16_t id = 31;

    EventConfigValueAperture() : Event(id){};
    EventConfigValueAperture(int32_t aperture);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t aperture;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueAperture, aperture);
};

struct EventConfigGetISO : public Event
{
    static constexpr uint16_t id = 32;

    EventConfigGetISO();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetISO);
};

struct EventConfigSetISO : public Event
{
    static constexpr uint16_t id = 33;

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
    static constexpr uint16_t id = 34;

    EventConfigValueISO() : Event(id){};
    EventConfigValueISO(int32_t iso);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    int32_t iso;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueISO, iso);
};

struct EventConfigGetBattery : public Event
{
    static constexpr uint16_t id = 35;

    EventConfigGetBattery();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetBattery);
};

struct EventConfigValueBattery : public Event
{
    static constexpr uint16_t id = 36;

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
    static constexpr uint16_t id = 37;

    EventConfigGetFocalLength();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetFocalLength);
};

struct EventConfigValueFocalLength : public Event
{
    static constexpr uint16_t id = 38;

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
    static constexpr uint16_t id = 39;

    EventConfigGetFocusMode();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetFocusMode);
};

struct EventConfigValueFocusMode : public Event
{
    static constexpr uint16_t id = 40;

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
    static constexpr uint16_t id = 41;

    EventConfigGetLongExpNR();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetLongExpNR);
};

struct EventConfigValueLongExpNR : public Event
{
    static constexpr uint16_t id = 42;

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
    static constexpr uint16_t id = 43;

    EventConfigGetVibRed();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetVibRed);
};

struct EventConfigValueVibRed : public Event
{
    static constexpr uint16_t id = 44;

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
    static constexpr uint16_t id = 45;

    EventConfigGetCaptureTarget();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetCaptureTarget);
};

struct EventConfigSetCaptureTarget : public Event
{
    static constexpr uint16_t id = 46;

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
    static constexpr uint16_t id = 47;

    EventConfigValueCaptureTarget() : Event(id){};
    EventConfigValueCaptureTarget(string target);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    string target;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueCaptureTarget, target);
};

struct EventConfigGetCameraMode : public Event
{
    static constexpr uint16_t id = 48;

    EventConfigGetCameraMode();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetCameraMode);
};

struct EventConfigValueCameraMode : public Event
{
    static constexpr uint16_t id = 49;

    EventConfigValueCameraMode() : Event(id){};
    EventConfigValueCameraMode(string target);

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    string target;

    JSON_EVENT_SERIALIZATION_INTRUSIVE(EventConfigValueCameraMode, target);
};

struct EventConfigGetCommon : public Event
{
    static constexpr uint16_t id = 50;

    EventConfigGetCommon();

    string name() const override;

    string to_string(int indent = -1) const override;

    nlohmann::json to_json() const override;

    JSON_EVENT_SERIALIZATION_INTRUSIVE_NOARGS(EventConfigGetCommon);
};