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
#include <string>

#include "Events.h"
#include "events/HSM.h"

#include "camera/CameraWrapper.h"
#include "utils/logger/PrintLogger.h"

using std::string;

class CameraController : public HSM<CameraController>
{
public:
    enum class CCState : uint8_t
    {
        DISCONNECTED = 0,
        READY = 1,
        CONNECTION_ERROR = 2,
        ERROR = 3,
        CAPTURING = 4,
        DOWNLOADING = 5
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

    CCState getState();
    void setDownloadDir(string download_dir);
private:
    void onStateChanged(CCState state);
    bool updateConfig();

    bool connect();

    bool getConfig(const EventPtr& ev);
    bool setConfig(const EventPtr& ev);

    CCState state;
    string download_dir;
    gphotow::CameraPath last_capture_path;
    bool do_download = false;
    bool low_latency = false;

    gphotow::CameraWrapper camera{};

    PrintLogger log = Logging::getLogger("CC_FSM");

    uint16_t state_error_recover_event_id = 0;
};