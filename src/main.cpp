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

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <scn/scn.h>

#include <argparse/argparse.hpp>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <thread>

#include "EventBroker.h"
#include "JsonLogSink.h"
#include "TcpLogSink.h"
#include "comm/CommManager.h"
#include "fsm/CameraController.h"
#include "fsm/ModeController.h"
#include "fsm/modes/Intervalometer.h"
#include "utils/EventSniffer.h"
#include "utils/debug/cli.h"
#include "utils/logger/PrintLogger.h"

using std::make_shared;
using std::shared_ptr;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;
using namespace std::filesystem;

PrintLogger elog = Logging::getLogger("event");
PrintLogger mlog = Logging::getLogger("main");

void initLogger(argparse::ArgumentParser& program);

void printEvent(const EventPtr& ev, uint8_t topic)
{
    LOG_EVENT(elog, "{} -> {}       {}", ev->name(), getTopicName(topic),
              ev->to_json().dump(-1));
}

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("camera-controller");

    program.add_argument("-n", "--network_log_sink")
        .help("ip_address:port of network log sink");

    program.add_argument("-l", "--log-folder")
        .help("Folder where to store logs");

    program.add_argument("-d", "--download_dir")
        .default_value(string{"."})
        .help("Directory where to save downloaded photos");

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

    initLogger(program);

    string dir = program.get<string>("-d");

    path p{dir};
    if (!is_directory(p))
    {
        LOG_ERR(mlog, "{} is not a directory", dir);
        std::exit(1);
    }

    LOG_DEBUG(mlog.getChild("arg_parse"), "Download directory = {}", dir);

    sBroker.start();
    EventSniffer sniffer{sEventBroker, &printEvent};
    CommManager comm(60099);

    ModeController mode_ctrl{};
    Intervalometer intervalometer{};

    CameraController camera{dir};

    mode_ctrl.start();
    intervalometer.start();
    camera.start();

    sBroker.post(EventCameraCmdConnect{}, TOPIC_CAMERA_CMD);

    CLI cli{};
    cli.start();

    for (;;)
        sleep_for(seconds(10));
    return 0;
}

void initLogger(argparse::ArgumentParser& program)
{
    if (auto fn = program.present("-n"))
    {
        string ip;
        uint16_t port;
        if (scn::scan(*fn, "{:[\\d.]}:{}", ip, port))
        {
            LOG_DEBUG(mlog.getChild("arg_parse"), "Network Log sink = {}:{}",
                      ip, port);
            Logging::addLogSink(make_shared<TcpLogSink>(ip, port));
        }
        else
        {
            LOG_ERR(mlog, "Invalid ip:port argument: {}", *fn);
            std::exit(1);
        }
    }
    else
    {
        LOG_DEBUG(mlog, "No network log sink!");
    }

    if (auto fn = program.present("-l"))
    {
        string datetime = fmt::format(
            "{::%Y_%m_%d_%H_%M_%S}",
            fmt::localtime(duration_cast<duration<int64_t>>(
                               high_resolution_clock::now().time_since_epoch())
                               .count()));
        path folder{*fn};

        LOG_DEBUG(mlog.getChild("arg_parse"), "Log folder:", *fn);

        try
        {
            string file = (folder / ("log_" + datetime + ".txt")).string();
            Logging::addLogSink(make_shared<FileLogSink>(file));
        }
        catch (std::system_error& se)
        {
            LOG_ERR(mlog, "Cannot creadte file log sink: {}", se.what());
            std::exit(1);
        }

        try
        {
            string file = (folder / ("log_" + datetime + ".log")).string();
            Logging::addLogSink(make_shared<JsonLogSink>(file));
        }
        catch (std::system_error& se)
        {
            LOG_ERR(mlog, "Cannot creadte JSON log sink: {}", se.what());
            std::exit(1);
        }
    }
    else
    {
        LOG_DEBUG(mlog, "No log folder specified!");
    }
}