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

#include <thread>
#include <chrono>
#include <memory>

#include "EventBroker.h"
#include "utils/logger/PrintLogger.h"
#include "fsm/CameraController.h"
#include "utils/EventSniffer.h"
#include "utils/debug/cli.h"

#include "TcpLogSink.h"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::shared_ptr;
using std::make_shared;

void printEvent(const EventPtr& ev, uint8_t topic)
{
    PrintLogger log = Logging::getLogger("event");
    LOG_EVENT(log, "Event {}:{} = {}", ev->name(), getTopicName(topic),  ev->to_string());
}

int main()
{
    CLI cli{};
    cli.start();

    PrintLogger log = Logging::getLogger("main");
    Logging::addLogSink(make_shared<TcpLogSink>("192.168.1.101", 60099));
    sBroker.start();

    EventSniffer sniffer{sEventBroker, &printEvent};
    
    CameraController camera;
    camera.start();

    for(;;)
        sleep_for(seconds(10));
    return 0;
}   