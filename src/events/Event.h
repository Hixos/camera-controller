/* Event
 *
 * Copyright (c) 2015-2018 Skyward Experimental Rocketry
 * Author: Matteo Michele Piazzolla
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

#include <fmt/core.h>
#include <stdint.h>

#include <cassert>
#include <memory>
#include <string>

using std::shared_ptr;
using std::string;

struct Event
{
    virtual ~Event(){};

    uint16_t getID() const { return _id; }

    virtual string name() const = 0;

    virtual string to_string() const = 0;

    void print() const { fmt::print(to_string()); }

protected:
    Event(uint16_t id) : _id(id) {}

private:
    uint16_t _id;
};

using EventPtr = shared_ptr<const Event>;

// Internal Events

struct EventSMEntry : public Event
{
    constexpr static uint16_t id = 0;

    EventSMEntry() : Event(id) {}

    string name() const override { return "EventSMEntry"; }
    string to_string() const override { return name(); }
};

struct EventSMExit : public Event
{
    constexpr static uint16_t id = 1;

    EventSMExit() : Event(id) {}

    string name() const override { return "EventSMExit"; }
    string to_string() const override { return name(); }
};

struct EventSMEmpty : public Event
{
    constexpr static uint16_t id = 2;

    EventSMEmpty() : Event(id) {}

    string name() const override { return "EventSMEmpty"; }
    string to_string() const override { return name(); }
};

struct EventSMInit : public Event
{
    constexpr static uint16_t id = 3;

    EventSMInit() : Event(id) {}

    string name() const override { return "EventSMInit"; }
    string to_string() const override { return name(); }
};

// Ready to use instances
extern const EventPtr C_EV_ENTRY;
extern const EventPtr C_EV_EXIT;
extern const EventPtr C_EV_EMPTY;
extern const EventPtr C_EV_INIT;
