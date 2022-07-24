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

#include "cli.h"

#include <cstdint>
#include <functional>
#include <map>
#include <string>

#include "Events.h"
#include "camera/CameraWrapper.h"
#include "events/EventBroker.h"

using scn::scan;
using scn::scan_value;
using std::function;
using std::map;
using std::string;

class CameraCLI
{
public:
    static bool parseCommand(string cmd)
    {
        string action;
        if (auto res = scan(cmd, "{}", action))
        {
            if (actions.count(action))
            {
                return actions.at(action)(res.range_as_string());
            }
        }
        return false;
    }

    static bool getConfig(string cmd)
    {
        string config;
        if (auto res = scan(cmd, "{}", config))
        {
            if (config_getters.count(config))
            {
                return config_getters.at(config)();
            }
        }

        return false;
    }

    static bool getChoices(string cmd)
    {
        string config;
        if (auto res = scan(cmd, "{}", config))
        {
            if (config_choices.count(config))
            {
                return config_choices.at(config)();
            }
        }

        return false;
    }

    static bool setConfig(string cmd)
    {
        string config;
        if (auto res = scan(cmd, "{}", config))
        {
            if (config_setters.count(config))
            {
                return config_setters.at(config)(res.range_as_string());
            }
        }

        return false;
    }

private:
    static const map<string, function<bool(string)>> actions;
    static const map<string, function<bool()>> config_getters;
    static const map<string, function<bool(string)>> config_setters;
    static const map<string, function<bool()>> config_choices;
};

const map<string, function<bool(string)>> CameraCLI::actions{
    {"get", &CameraCLI::getConfig},
    {"choices", &CameraCLI::getChoices},
    {"set", &CameraCLI::setConfig},
    {"low_latency",
     [](string cmd) {
         if (auto res = scan_value<bool>(cmd))
         {
             sBroker.post(EventCameraCmdLowLatency{res.value()},
                          TOPIC_REMOTE_CMD);
             return true;
         }
         return false;
     }},
    {"capture",
     [](string cmd) {
         sBroker.post(EventCameraCmdCapture{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"download",
     [](string cmd) {
         if (auto res = scan_value<bool>(cmd))
         {
             sBroker.post(EventCameraCmdDownload{res.value()},
                          TOPIC_REMOTE_CMD);
             return true;
         }
         return false;
     }},
    {"connect",
     [](string cmd) {
         sBroker.post(EventCameraCmdConnect{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"disconnect",
     [](string cmd) {
         sBroker.post(EventCameraCmdDisconnect{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"recover",
     [](string cmd) {
         sBroker.post(EventCameraCmdRecoverError{}, TOPIC_REMOTE_CMD);
         return true;
     }},

};

const map<string, function<bool()>> CameraCLI::config_getters{
    {"common",
     []() {
         sBroker.post(EventConfigGetAll{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"shutter_speed",
     []() {
         sBroker.post(EventConfigGetShutterSpeed{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"aperture",
     []() {
         sBroker.post(EventConfigGetAperture{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"iso",
     []() {
         sBroker.post(EventConfigGetISO{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"battery",
     []() {
         sBroker.post(EventConfigGetBattery{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"focal_length",
     []() {
         sBroker.post(EventConfigGetFocalLength{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"focus_mode",
     []() {
         sBroker.post(EventConfigGetFocusMode{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"long_exp_nr",
     []() {
         sBroker.post(EventConfigGetLongExpNR{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"light_meter",
     []() {
         sBroker.post(EventConfigGetLightMeter{}, TOPIC_REMOTE_CMD);
         return true;
     }},
};

const map<string, function<bool()>> CameraCLI::config_choices{
    {"shutter_speed",
     []() {
         sBroker.post(EventConfigGetChoicesShutterSpeed{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"aperture",
     []() {
         sBroker.post(EventConfigGetChoicesAperture{}, TOPIC_REMOTE_CMD);
         return true;
     }},
    {"iso", []() {
         sBroker.post(EventConfigGetChoicesISO{}, TOPIC_REMOTE_CMD);
         return true;
     }}};

const map<string, function<bool(string)>> CameraCLI::config_setters{
    {"shutter_speed",
     [](string cmd) {
         if (auto res = scan_value<float>(cmd))
         {
             sBroker.post(
                 EventConfigSetShutterSpeed{(int32_t)(res.value() * 1000000)},
                 TOPIC_REMOTE_CMD);
             return true;
         }
         return false;
     }},
    {"aperture",
     [](string cmd) {
         if (auto res = scan_value<int32_t>(cmd))
         {
             sBroker.post(EventConfigSetAperture{res.value()},
                          TOPIC_REMOTE_CMD);
             return true;
         }
         return false;
     }},
    {"iso",
     [](string cmd) {
         if (auto res = scan_value<int32_t>(cmd))
         {
             sBroker.post(EventConfigSetISO{res.value()}, TOPIC_REMOTE_CMD);
             return true;
         }
         return false;
     }},
    {"capture_target", [](string cmd) {
         if (auto res = scan_value<string>(cmd))
         {
             if (res.value() == "sd")
                 sBroker.post(EventConfigSetCaptureTarget{"Memory card"},
                              TOPIC_REMOTE_CMD);
             else if (res.value() == "ram")
                 sBroker.post(EventConfigSetCaptureTarget{"Internal RAM"},
                              TOPIC_REMOTE_CMD);
             else
                 return false;
             return true;
         }
         return false;
     }}};

class LogCLI
{
public:
    static bool parseCommand(string cmd)
    {
        string action;
        if (auto res = scan(cmd, "{}", action))
        {
            if (action == "level")
            {
                return setLevel(res.range_as_string());
            }
        }
        return false;
    }

private:
    static bool setLevel(string cmd)
    {
        string level;
        if (auto res = scan(cmd, "{}", level))
        {
            if (levels.count(level))
            {
                levels.at(level)();
                return true;
            }
        }
        return false;
    }

    static const map<string, function<void()>> levels;
};

const map<string, function<void()>> LogCLI::levels{
    {"debug",
     [] { Logging::getStdOutLogSink().setLevel(LogLevel::LOGL_DEBUG); }},
    {"event",
     [] { Logging::getStdOutLogSink().setLevel(LogLevel::LOGL_EVENT); }},
    {"info", [] { Logging::getStdOutLogSink().setLevel(LogLevel::LOGL_INFO); }},
    {"warning",
     [] { Logging::getStdOutLogSink().setLevel(LogLevel::LOGL_WARNING); }},
    {"error",
     [] { Logging::getStdOutLogSink().setLevel(LogLevel::LOGL_ERROR); }},
    {"critical",
     [] { Logging::getStdOutLogSink().setLevel(LogLevel::LOGL_CRITICAL); }},
};

class IntervalometerCLI
{
public:
    static bool parseCommand(string cmd)
    {
        string action;
        auto res1 = scan(cmd, "{}", action);
        if (res1)
        {
            if (action == "start")
            {
                int32_t interval;
                int32_t total_captures;
                if (auto res2 = scan(res1.range_as_string(), "{} {}", interval,
                                     total_captures))
                {
                    sEventBroker.post(
                        EventModeIntervalometer{interval, total_captures},
                        TOPIC_REMOTE_CMD);
                    return true;
                }
            }
        }
        return false;
    }
};

class ModeCLI
{
public:
    static bool parseCommand(string cmd)
    {
        string action;
        if (auto res = scan(cmd, "{}", action))
        {
            if (action == "stop")
            {
                sEventBroker.post(EventModeStop{}, TOPIC_REMOTE_CMD);
                return true;
            }

            if (action == "get")
            {
                sEventBroker.post(EventGetCurrentMode{}, TOPIC_REMOTE_CMD);
                return true;
            }

            if (action == "interv")
            {
                return IntervalometerCLI::parseCommand(res.range_as_string());
            }
        }
        return false;
    }
};

CLI::CLI() {}

CLI::~CLI() { stop(); }

void CLI::run()
{
    // clang-format off
    static map<string, function<bool(string)>> targets{
        {"camera", &CameraCLI::parseCommand},
        {"log", &LogCLI::parseCommand},
        {"mode", &ModeCLI::parseCommand},
        {"exit", [&](string line){ LOG_INFO(log, "Goodbye!"); exit(0); return true; }}
    };
    // clang-format on

    while (!shouldStop())
    {
        string line;
        if (scn::getline(scn::cstdin(), line))
        {
            string target;
            if (auto res = scan(line, "{}", target))
            {
                if (targets.count(target))
                {
                    if (!targets.at(target)(res.range_as_string()))
                    {
                        LOG_ERR(log, "Invalid command");
                    }
                }
            }
        }
        scn::cstdin().sync();
    }
}
