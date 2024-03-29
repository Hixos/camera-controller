/* Copyright (c) 2015-2018 Skyward Experimental Rocketry
 * Authors: Luca Erbetta, Matteo Michele Piazzolla
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <stdint.h>
#include <fmt/core.h>

#include <chrono>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <type_traits>
#include <vector>

#include "EventBase.h"
#include "events/EventHandler.h"
#include "utils/ActiveObject.h"
#include "utils/Singleton.h"

using std::map;
using std::vector;

using std::condition_variable;
using std::lock_guard;
using std::unique_lock;

using std::mutex;

using std::chrono::milliseconds;
using std::chrono::system_clock;

// Minimum guaranteed delay for an event posted with postDelayed(...) in ms
static constexpr unsigned int EVENT_BROKER_MIN_DELAY = 50;

/**
 * The EventBroker class implements the pub-sub paradigm to dispatch events to
 * multiple objects. An object of type FSM can subscribe to a topic in the
 * public topics enum and publish an event into it. The event will be posted in
 * to each FSM object subscribed to that specific topic.
 */
class EventBroker : public Singleton<EventBroker>, public ActiveObject
{
    friend class Singleton<EventBroker>;

public:
    /**
     * Posts an event to the specified topic.
     * @param ev
     * @param topic
     */
    void post(const EventPtr& ev, uint8_t topic);

    /**
     * Posts an event to the specified topic.
     * @param ev
     * @param topic
     */
    template <
        typename EventClass,
        typename = std::enable_if_t<std::is_base_of<Event, EventClass>::value>>
    void post(EventClass&& ev, uint8_t topic)
    {
        post(std::make_shared<const EventClass>(ev), topic);
    }

    /**
     * Posts an event to the specified topic.
     * @param ev
     * @param topic
     */
    template <
        typename EventClass,
        typename = std::enable_if_t<std::is_base_of<Event, EventClass>::value>>
    void post(const EventClass& ev, uint8_t topic)
    {
        post(std::make_shared<const EventClass>(ev), topic);
    }

    /**
     * Posts an event after the specified delay.
     *
     * @param event
     * @param topic
     * @param delay_ms Delay in milliseconds.
     * @return Unique id of the delayed event.
     */
    uint16_t postDelayed(const EventPtr& ev, uint8_t topic,
                         unsigned int delay_ms);

    /**
     * Posts an event after the specified delay.
     *
     * @param event
     * @param topic
     * @param delay_ms Delay in milliseconds.
     * @return Unique id of the delayed event.
     */
    template <
        typename EventClass,
        typename = std::enable_if_t<std::is_base_of<Event, EventClass>::value>>
    uint16_t postDelayed(const EventClass&& ev, uint8_t topic,
                         unsigned int delay_ms)
    {
        return postDelayed(std::make_shared<const EventClass>(ev), topic,
                           delay_ms);
    }

    /**
     * Removes a delayed event before it is posted.
     * @param id The id returned by postDelayed(...).
     */
    void removeDelayed(uint16_t id);

    /**
     * Subscribe to a specific topic.
     * DO NOT call it in response to an event, or it will cause a deadlock.
     * @param subscriber
     * @param topic
     */
    void subscribe(EventHandlerBase* subscriber, uint8_t topic);

    /**
     * @brief Unsubscribe an EventHandler from a specific topic
     * This function should be used only for testing purposes
     * @param subscriber
     * @param topic
     */
    void unsubscribe(EventHandlerBase* subscriber, uint8_t topic);

    /**
     * @brief Unsubribe an EventHandler from all the topics it is subscribed to.
     * This function should be used only for testing purposes
     * @param subscriber
     */
    void unsubscribe(EventHandlerBase* subscriber);

    /**
     * @brief Unschedules all pending events.
     * This function should be used only for testing purposes
     */
    void clearDelayedEvents();

    /**
     * @brief Construct a new Event Broker object.
     * Public access required for testing purposes. Use the singleton interface
     * to access this class in production code.
     *
     */
    EventBroker();

    ~EventBroker();

    void stop() override;

private:
    /**
     * Private structure for holding a delayed event data in the list.
     */
    struct DelayedEvent
    {
        uint16_t sched_id;
        EventPtr event;
        uint8_t topic;
        system_clock::time_point deadline;

        DelayedEvent(uint16_t sched_id, EventPtr event, uint8_t topic,
                     system_clock::time_point deadline)
            : sched_id(sched_id), event(event), topic(topic), deadline(deadline)
        {
        }
    };

    /**
     * Active Object run
     */
    void run() override;

    void deleteSubscriber(vector<EventHandlerBase*>& sub_vector,
                          EventHandlerBase* subscriber);

    vector<DelayedEvent> delayed_events;
    mutex mtx_delayed_events;
    condition_variable cv_delayed_events;

    map<uint8_t, vector<EventHandlerBase*>> subscribers;
    mutex mtx_subscribers;

    uint16_t eventCounter = 0;
};

#define sEventBroker Singleton<EventBroker>::getInstance()
#define sBroker Singleton<EventBroker>::getInstance()
