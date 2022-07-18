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

#include <chrono>
#include <memory>
#include <thread>

#include "PrintLogger.h"
#include "TcpLogSink.h"

using std::shared_ptr;
using std::make_shared;

using std::chrono::seconds;
using std::this_thread::sleep_for;

int main()
{
    shared_ptr<TcpLogSink> tcp_sink =
        make_shared<TcpLogSink>("192.168.1.102", 19996);

    auto sink = std::static_pointer_cast<LogSink>(tcp_sink);
    Logging::addLogSink(sink);

    auto log = Logging::getLogger("Prova");

    for (int i = 0; i < 3000; ++i)
    {
        LOG_INFO(log, "Ciao abc {}", i);
        sleep_for(seconds(3));
    }

    sleep_for(seconds(1));
    tcp_sink->stop();
    fmt::print("Stopped!\n");

    
}