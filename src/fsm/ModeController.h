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

#include <memory>
#include <mutex>
#include <string>

#include "EventBroker.h"
#include "Events.h"
#include "HSM.h"
#include "PrintLogger.h"
#include "utils/collections/CircularBuffer.h"

using std::dynamic_pointer_cast;
using std::lock_guard;
using std::mutex;
using std::string;

class ModeController : public HSM<ModeController>
{
public:
    ModeController() : HSM<ModeController>(&ModeController::stateInit)
    {
        sEventBroker.subscribe(this, TOPIC_REMOTE_CMD);
        sEventBroker.subscribe(this, TOPIC_MODE_CONTROLLER);
        sEventBroker.subscribe(this, TOPIC_HEARTBEAT);
    }

    ~ModeController() { sEventBroker.unsubscribe(this); }

private:
    class EventPassThrough : public EventHandler
    {
    public:
        EventPassThrough()
        {
            sBroker.subscribe(this, TOPIC_REMOTE_CMD);
            start();
        }

        ~EventPassThrough()
        {
            sBroker.unsubscribe(this);
            stop();
        }

        void setPassThough(bool enable)
        {
            lock_guard<mutex> lock(m);
            pass_though_enabled = enable;
            if (enable)
            {
                while (!deferred.isEmpty())
                {
                    sEventBroker.post(deferred.pop(), TOPIC_CAMERA_CMD);
                }
            }
        }

    protected:
        void handleEvent(const EventPtr& ev) override
        {
            lock_guard<mutex> lock(m);
            if (pass_though_enabled)
                sEventBroker.post(ev, TOPIC_CAMERA_CMD);
            else
                deferred.put(ev);
        }

    private:
        mutex m;
        bool pass_though_enabled = true;
        CircularBuffer<EventPtr, 100> deferred;
    };

    State stateInit(const EventPtr& ev)
    {
        return transition(&ModeController::stateSuper);
    }

    State stateSuper(const EventPtr& ev)
    {
        auto slog      = log.getChild("Super");
        State retState = State::HANDLED;
        switch (ev->getID())
        {
            case EventSMEntry::id:
                LOG_STATE(slog, "ENTRY");
                sBroker.postDelayed(EventHeartBeat{}, TOPIC_HEARTBEAT, 1000);
                break;
            case EventSMInit::id:
                retState = transition(&ModeController::stateModeSelection);
                break;
            case EventSMExit::id:
                LOG_STATE(slog, "EXIT");
                break;
            case EventGetCurrentMode::id:
                sBroker.post(EventValueCurrentMode{current_mode},
                             TOPIC_MODE_STATE);
                break;
            case EventEnableEventPassThrough::id:
                pass_through.setPassThough(true);
                break;
            case EventDisableEventPassThrough::id:
                pass_through.setPassThough(false);
                break;
            case EventHeartBeat::id:
                sBroker.postDelayed(EventHeartBeat{}, TOPIC_HEARTBEAT, 1000);
                break;
            default:
                retState = tran_super(&ModeController::Hsm_top);
                break;
        }
        return retState;
    }

    State stateModeSelection(const EventPtr& ev)
    {
        auto slog      = log.getChild("Selection");
        State retState = State::HANDLED;
        switch (ev->getID())
        {
            case EventSMEntry::id:
                current_mode = "Manual";
                sBroker.post(EventValueCurrentMode{current_mode},
                             TOPIC_MODE_STATE);
                LOG_STATE(slog, "ENTRY");
                break;
            case EventSMInit::id:
                break;
            case EventSMExit::id:
                LOG_STATE(slog, "EXIT");
                break;
            case EventModeIntervalometer::id:
            {
                current_mode = "Intervalometer";
                sBroker.post(EventValueCurrentMode{current_mode},
                             TOPIC_MODE_STATE);
                auto ie =
                    dynamic_pointer_cast<const EventModeIntervalometer>(ev);
                sEventBroker.post(EventIntervalometerStart{ie->intervalms,
                                                           ie->total_captures},
                                  TOPIC_MODE_FSM);
                LOG_INFO(slog,
                         "Starting intervalometer. interval: {:.1f}s, num: {}",
                         ie->intervalms / 1000.0f, ie->total_captures);
                retState = transition(&ModeController::stateRunning);
                break;
            }
            default:
                retState = tran_super(&ModeController::stateSuper);
                break;
        }
        return retState;
    }

    State stateRunning(const EventPtr& ev)
    {
        auto slog = log.getChild("Running");

        State retState = State::HANDLED;
        switch (ev->getID())
        {
            case EventSMEntry::id:
                LOG_STATE(slog, "ENTRY");
                break;
            case EventSMInit::id:
                break;
            case EventSMExit::id:
                LOG_STATE(slog, "EXIT");
                break;
            case EventModeStopped::id:
                retState = transition(&ModeController::stateModeSelection);
                break;
            default:
                retState = tran_super(&ModeController::stateSuper);
                break;
        }
        return retState;
    }

private:
    EventPassThrough pass_through{};
    string current_mode = "Manual";

    PrintLogger log = Logging::getLogger("ModCtrl");
};