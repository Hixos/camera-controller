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

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>

#include "Events.h"
#include "camera/CameraWrapper.h"
#include "events/HSM.h"
#include "utils/logger/PrintLogger.h"

using std::function;
using std::map;
using std::string;

class CameraController : public HSM<CameraController>
{
public:
    enum class CCState : uint8_t
    {
        DISCONNECTED     = 0,
        READY            = 1,
        CONNECTION_ERROR = 2,
        ERROR            = 3,
        CAPTURING        = 4,
        DOWNLOADING      = 5
    };

    CameraController(string download_dir = ".");

    State stateInit(const EventPtr& ev);
    State stateSuper(const EventPtr& ev);
    State stateDisconnected(const EventPtr& ev);
    State stateConnected(const EventPtr& ev);
    State stateReady(const EventPtr& ev);
    State stateConnectionError(const EventPtr& ev);
    State stateError(const EventPtr& ev);

    State stateCapturing(const EventPtr& ev);
    State stateDownloading(const EventPtr& ev);

    void setDownloadDir(string download_dir);

private:
    enum class ConfigEventHandleResult
    {
        HANDLED,
        UNHANDLED,
        ERROR
    };

    void onStateChanged(CCState state);
    void onCameraConnected(bool connected);
    void getState();

    bool connect();
    void checkConnection();

    State handleConfigGetSet(const EventPtr& ev);
    ConfigEventHandleResult getConfig(const EventPtr& ev);
    ConfigEventHandleResult setConfig(const EventPtr& ev);
    bool getAllConfig();

    CCState state;
    bool camera_connected;
    string download_dir;
    gphotow::CameraPath last_capture_path;
    bool do_download = false;
    bool low_latency = false;

    gphotow::CameraWrapper camera{};

    PrintLogger log = Logging::getLogger("CC_FSM");

    uint16_t state_error_recover_event_id = 0;

    static const map<uint16_t, function<void(CameraController&)>>
        config_getters;
    static const map<uint16_t,
                     function<void(CameraController&, const EventPtr&)>>
        config_setters;
    static const map<CCState, string> state_names;
};