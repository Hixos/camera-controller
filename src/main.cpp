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

#include <scn/scn.h>

#include <argparse/argparse.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "EventBroker.h"
#include "TcpLogSink.h"
#include "fsm/CameraController.h"
#include "comm/CommManager.h"
#include "comm/UDPEchoServer.h"

#include "utils/EventSniffer.h"
#include "utils/debug/cli.h"
#include "utils/logger/PrintLogger.h"

using std::make_shared;
using std::shared_ptr;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;

PrintLogger elog = Logging::getLogger("event");
PrintLogger mlog = Logging::getLogger("main");

void printEvent(const EventPtr& ev, uint8_t topic)
{

    LOG_EVENT(elog, "{} -> {}       {}", ev->name(), getTopicName(topic),
              ev->to_json().dump(-1));
}

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("camera-controller");

    program.add_argument("-l", "--log-sink")
        .help("ip_address:port of log sink");

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err)
    {
        // auto arg_log = mlog.getChild("argparse");
        LOG_ERR(mlog, err.what());
        std::cerr << program;
        std::exit(1);
    }

    if (auto fn = program.present("-l"))
    {
        string ip;
        uint16_t port;
        if (scn::scan(*fn, "{:[\\d.]}:{}", ip, port))
        {
            LOG_DEBUG(mlog.getChild("arg_parse"), "Log sink = {}:{}", ip, port);
            // Logging::addLogSink(make_shared<TcpLogSink>(ip, port));
        }
        else
        {
            LOG_ERR(mlog, "Invalid ip:port argument: {}", *fn);
            std::exit(1);
        }
    }
    else
    {
        LOG_DEBUG(mlog, "No log sink!");
    }

    sBroker.start();
    EventSniffer sniffer{sEventBroker, &printEvent};

    CameraController camera;
    camera.start();

    CLI cli{};
    cli.start();

    CommManager comm(60099);
    UDPEchoServer echo("0.0.0.0", 60050, false);

    for (;;)
        sleep_for(seconds(10));
    return 0;
}