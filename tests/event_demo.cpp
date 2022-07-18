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

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <string>
#include <type_traits>
// #include <fmt/core.h>

struct EventBase
{
    virtual ~EventBase(){};

    uint16_t getID() { return _id; }

    template <typename T>
    T* cast_to()
    {
        static_assert(std::is_base_of_v<EventBase, T>);
        return dynamic_cast<T*>(this);
    }

protected:
    EventBase(uint16_t id) : _id(id) {}

private:
    uint16_t _id;
};

struct EventPayload : public EventBase
{
    EventPayload(int payload) : EventBase(id), payload(payload) {}

    virtual ~EventPayload() {}

    int payload;

    static constexpr uint16_t id = 1;
};

struct EventPayload2 : public EventBase
{
    EventPayload2(int payload) : EventBase(id), payload(payload) {}
    virtual ~EventPayload2() {}

    int payload;

    static constexpr uint16_t id = 2;
};

struct Prova
{
    int a = 4;
};

int main()
{
    EventBase* e = new EventPayload(10);

    // Event* e = p;

    switch (e->getID())
    {
        case EventPayload::id:
            printf("EventPayload\n");
            break;
        case EventPayload2::id:
            printf("EventPayload2\n");
            break;
        default:
            break;
    }

    delete e;
}