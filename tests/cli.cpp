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

#include <fmt/core.h>
#include <scn/scn.h>

#include <functional>
#include <map>
#include <string>

#include "camera/CameraWrapper.h"

using std::bind;
using std::function;
using std::map;
using std::string;

using namespace gphotow;

void getConfig();
void setConfig();
void capture();
void connect();
void disconnect();

// clang-format off
map<string, function<void()>> actions{
    {"get", &getConfig},
    {"set", &setConfig},
    {"capture", &capture},
    {"connect", &connect},
    {"disconnect", &disconnect},
    {"exit", []{exit(0);}}
    };
// clang-format on

gphotow::CameraWrapper camera;

int main()
{
    string action;

    while (true)
    {
        auto res = scn::prompt(
            "What to do? (get aperture /  set shutter_speed 42s / capture): ",
            "{}", action);

        if (res)
        {
            if (actions.count(action) > 0)
            {
                actions[action]();
            }
        }
    }
}

void getConfig()
{
    //clang-format off
    static map<string, function<string()>> getters{
        {"shutter_speed",
         [&] {
             float ss = camera.getShutterSpeed().shutter_speed / 1000000.0f;
             if (ss >= 1)
             {
                 return fmt::format("{:.1f} s", ss);
             }
             else
             {
                 return fmt::format("1/{:.0f} s", 1 / ss);
             }
         }},
        {"aperture",
         [&] {
             return fmt::format("f-{:.2f}", camera.getAperture() / 100.0f);
         }},
        {"iso", [&] { return fmt::format("{}", camera.getISO()); }},
        {"info", bind(&CameraWrapper::getCameraInfo, &camera)},
        {"battery",
         [&] { return fmt::format("{}%", camera.getBatteryPercent()); }},
        {"focal_length",
         [&] { return fmt::format("{} mm", camera.getFocalLength()); }},
        {"exp_program", bind(&CameraWrapper::getExposureProgram, &camera)},
        {"long_exp_nr", bind(&CameraWrapper::getLongExpNR, &camera)}};
    // clang-format on
    string config;
    auto res = scn::input("{}", config);

    if (res)
    {
        if (getters.count(config) > 0)
        {
            string val;
            try
            {
                val = getters[config]();
            }
            catch (std::exception& e)
            {
                fmt::print("Error getting value: {}\n", e.what());
            }
            fmt::print("{}\n", val);
        }
        else
        {
            fmt::print("Unknown config: {}\n", config);
        }
    }
}

void setConfig()
{
    static map<string, function<void(string)>> setters{
        {"shutter_speed",
         [&](string val) {
             float s;
             if (scn::scan(val, "{}", s))
                 camera.setShutterSpeed(s * 1000 * 1000);
             else
                 throw std::invalid_argument(
                     fmt::format("Cannot parse shutter speed \"{}\"", val));
         }},
        {"aperture",
         [&](string val) {
             float fnum;
             if (scn::scan(val, "{}", fnum))
                 camera.setAperture(fnum * 100);
             else
                 throw std::invalid_argument(
                     fmt::format("Cannot parse aperture \"{}\"", val));
         }},
        {"iso", [&](string val) {
             int iso;
             if (scn::scan(val, "{}", iso))
                 camera.setISO(iso);
             else
                 throw std::invalid_argument(
                     fmt::format("Cannot parse ISO \"{}\"", val));
         }}};

    string config;
    string value;
    auto res = scn::input("{} {}", config, value);

    if (res)
    {
        if (setters.count(config) > 0)
        {
            try
            {
                setters[config](value);
                fmt::print("{} set to {}\n", config, value);
            }
            catch (std::exception& e)
            {
                fmt::print("Error setting value: {}\n", e.what());
            }
        }
        else
        {
            fmt::print("Unknown config: {}\n", config);
        }
    }
}

void capture()
{
    try
    {
        camera.capture();
    }
    catch (std::exception& e)
    {
        fmt::print("Capture error: {}\n", e.what());
    }
}

void connect()
{
    try
    {
        camera.connect();
        fmt::print("Connected: {}\n", camera.getCameraInfo());
    }
    catch (std::exception& e)
    {
        fmt::print("Connection error: {}\n", e.what());
    }
}
void disconnect()
{
    try
    {
        camera.disconnect();
    }
    catch (std::exception& e)
    {
        fmt::print("Disconnection egetLongExpNRrror: {}\n", e.what());
    }
}