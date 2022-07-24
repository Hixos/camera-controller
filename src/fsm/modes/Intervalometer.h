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
#include <string>

#include "EventBroker.h"
#include "Events.h"
#include "HSM.h"
#include "PrintLogger.h"

using std::dynamic_pointer_cast;
using std::string;

class Intervalometer : public HSM<Intervalometer, 100>
{
    using Super = HSM<Intervalometer, 100>;

public:
    Intervalometer() : Super(&Intervalometer::stateInit)
    {
        sEventBroker.subscribe(this, TOPIC_MODE_FSM);
        sEventBroker.subscribe(this, TOPIC_REMOTE_CMD);
    }

    ~Intervalometer() { sEventBroker.unsubscribe(this); }

private:
    State stateInit(const EventPtr& ev)
    {
        return transition(&Intervalometer::stateSuper);
    }

    State stateSuper(const EventPtr& ev)
    {
        auto slog      = log.getChild("Super");
        State retState = State::HANDLED;
        switch (ev->getID())
        {
            case EventSMEntry::id:
                LOG_STATE(slog, "ENTRY");
                break;
            case EventSMInit::id:
                retState = transition(&Intervalometer::stateReady);
                break;
            case EventSMExit::id:
                LOG_STATE(slog, "EXIT");
                break;
            case EventGetCurrentMode::id:
                sEventBroker.post(
                    EventIntervalometerState{state, interval, num_captures,
                                             total_captures},
                    TOPIC_MODE_STATE);
            default:
                retState = tran_super(&Intervalometer::Hsm_top);
                break;
        }
        return retState;
    }

    State stateReady(const EventPtr& ev)
    {
        auto slog      = log.getChild("Ready");
        State retState = State::HANDLED;
        switch (ev->getID())
        {
            case EventSMEntry::id:
                onStateChange("ready");
                sEventBroker.unsubscribe(this, TOPIC_CAMERA_EVENT);
                LOG_STATE(slog, "ENTRY");
                break;
            case EventSMInit::id:
                break;
            case EventSMExit::id:
                sEventBroker.subscribe(this, TOPIC_CAMERA_EVENT);
                LOG_STATE(slog, "EXIT");
                break;
            case EventIntervalometerStart::id:
            {
                auto s =
                    dynamic_pointer_cast<const EventIntervalometerStart>(ev);
                interval       = s->intervalms;
                total_captures = s->total_captures;
                num_captures   = 0;
                retState       = transition(&Intervalometer::stateCapturing);
                break;
            }
            default:
                retState = tran_super(&Intervalometer::stateSuper);
                break;
        }
        return retState;
    }

    State stateRunning(const EventPtr& ev)
    {
        auto slog      = log.getChild("Running");
        State retState = State::HANDLED;
        switch (ev->getID())
        {
            case EventSMEntry::id:
                LOG_STATE(slog, "ENTRY");
                break;
            case EventSMInit::id:
                retState = transition(&Intervalometer::stateCapturing);
                break;
            case EventSMExit::id:
                sEventBroker.post(EventModeStopped{}, TOPIC_MODE_CONTROLLER);
                LOG_STATE(slog, "EXIT");
                break;
            case EventModeStop::id:
                retState = transition(&Intervalometer::stateReady);
                break;
            default:
                retState = tran_super(&Intervalometer::stateSuper);
                break;
        }
        return retState;
    }

    State stateCapturing(const EventPtr& ev)
    {
        auto slog      = log.getChild("Capturing");
        State retState = State::HANDLED;
        switch (ev->getID())
        {
            case EventSMEntry::id:
                deadline_expired = false;
                onStateChange("capturing");
                sEventBroker.post(EventCameraCmdCapture{}, TOPIC_CAMERA_CMD);
                if (interval > 0)
                {
                    sBroker.postDelayed(EventIntervalometerDeadlineExpired{},
                                        TOPIC_MODE_FSM, interval);
                }
                LOG_STATE(slog, "ENTRY");
                break;
            case EventSMInit::id:
                break;
            case EventSMExit::id:
                LOG_STATE(slog, "EXIT");
                break;
            case EventCameraCaptureDone::id:
                ++num_captures;
                LOG_INFO(slog, "Intervalometer progress: {}/{}", num_captures,
                         total_captures);
                if (num_captures >= total_captures && total_captures > 0)
                {
                    retState = transition(&Intervalometer::stateReady);
                    break;
                }
                if (interval < 0 || deadline_expired)
                {
                    retState = transition(&Intervalometer::stateCapturing);
                }
                else
                {
                    retState = transition(&Intervalometer::stateWaiting);
                }
                break;
            case EventIntervalometerDeadlineExpired::id:
                deadline_expired = true;
                break;
            case EventCameraError::id:
                history  = &Intervalometer::stateCapturing;
                retState = transition(&Intervalometer::stateError);
                break;
            default:
                retState = tran_super(&Intervalometer::stateRunning);
                break;
        }
        return retState;
    }

    State stateWaiting(const EventPtr& ev)
    {
        auto slog      = log.getChild("Waiting");
        State retState = State::HANDLED;
        switch (ev->getID())
        {
            case EventSMEntry::id:
                onStateChange("waiting");
                LOG_STATE(slog, "ENTRY");
                break;
            case EventSMInit::id:
                break;
            case EventSMExit::id:
                LOG_STATE(slog, "EXIT");
                break;
            case EventIntervalometerDeadlineExpired::id:
                retState = transition(&Intervalometer::stateCapturing);
                break;
            case EventCameraError::id:
                history  = &Intervalometer::stateWaiting;
                retState = transition(&Intervalometer::stateError);
                break;
            default:
                retState = tran_super(&Intervalometer::stateRunning);
                break;
        }
        return retState;
    }

    State stateError(const EventPtr& ev)
    {
        auto slog      = log.getChild("Error");
        State retState = State::HANDLED;
        switch (ev->getID())
        {
            case EventSMEntry::id:
                LOG_STATE(slog, "ENTRY");
                onStateChange("error");
                break;
            case EventSMInit::id:
                break;
            case EventSMExit::id:
                LOG_STATE(slog, "EXIT");
                break;
            case EventIntervalometerDeadlineExpired::id:
                deadline_expired = true;
                break;
            case EventCameraConnected::id:
                if (deadline_expired)
                    retState = transition(&Intervalometer::stateCapturing);
                else
                    retState = transition(history);
                break;
            default:
                retState = tran_super(&Intervalometer::stateRunning);
                break;
        }
        return retState;
    }

private:
    void onStateChange()
    {
        sEventBroker.post(
            EventIntervalometerState{state, interval, num_captures,
                                     total_captures},
            TOPIC_MODE_STATE);
    }
    void onStateChange(string state)
    {
        this->state = state;
        onStateChange();
    }
    string state           = "ready";
    int32_t interval       = 0;
    int32_t num_captures   = 0;
    int32_t total_captures = 0;

    State (Intervalometer::*history)(const EventPtr& ev);

    bool deadline_expired = false;

    PrintLogger log = Logging::getLogger("Interv");
};