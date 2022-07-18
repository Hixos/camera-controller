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

#include "json_auto.h"
#include <fmt/core.h>
#include "Events.h"
#include <cassert>

Test::Test(int a, std::string b, float c) : a(a), b(b), c(c) {}

using namespace nlohmann;
int main()
{
    Test t{1, "ciao", 2.2f};

    json j(t);
    fmt::print("{}\n", j.dump(4));

    Test t2 = j.get<Test>();
    assert(t == t2);

     EventConfigSetShutterSpeed ev{1234};

    json j2 = ev.to_json();

    fmt::print("{}\n", j2.dump(4));

    EventPtr ev2 = jsonToEvent(j2);

    auto ev2_c = std::dynamic_pointer_cast<const EventConfigSetShutterSpeed>(ev2);

    assert(ev.shutter_speed == ev2_c->shutter_speed);

}