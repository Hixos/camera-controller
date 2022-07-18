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

#include "TcpLogSink.h"

#include <arpa/inet.h>
#include <fmt/core.h>

#include <chrono>
#include <csignal>
#include <nlohmann/json.hpp>
#include <thread>

#include "PrintLoggerData.h"

using std::chrono::milliseconds;
using std::this_thread::sleep_for;

TcpLogSink::TcpLogSink(string ip, uint16_t port)
    : ip(ip), port(port), sockInit(), conn()
{
    start();
}

TcpLogSink::~TcpLogSink()
{
    stop();
}

void TcpLogSink::stop()
{
    if (started && !stopped)
    {
        should_stop = true;
        conn.shutdown();
        LogRecord r{.level = -666};
        records.put(r);
        if (thread_obj->joinable())
            thread_obj->join();
        stopped = true;
    }
}

void TcpLogSink::logImpl(const LogRecord& record) { records.put(record); }

void TcpLogSink::run()
{
    while (!shouldStop())
    {
        if (curr_record_sent)
        {
            curr_record      = records.popBlocking();
            curr_record_sent = false;
        }

        // stop() was called
        if (curr_record.level == -666 || shouldStop())
            break;

        if (!conn.is_connected())
        {
            if (!conn.connect(sockpp::inet_address(ip, port)))
            {
                sleep_for(milliseconds(200));
            }
        }
        else
        {
            auto buf = packJson(recordToJson(curr_record));

            int res = conn.write_n(buf.first.get(), buf.second);

            if (res < 0 || (size_t)res != buf.second)
            {
                conn.close();
            }
            else
            {
                curr_record_sent = true;
            }
        }
    }
}

pair<BufPtr, size_t> TcpLogSink::packJson(string json)
{
    uint32_t len = json.length();
    BufPtr buf   = std::make_unique<uint8_t[]>(len + 4);

    uint32_t len_net = htonl(len);

    memcpy(buf.get(), &len_net, sizeof(len_net));
    memcpy(buf.get() + 4, json.data(), len);

    return {std::move(buf), len + 4};
}

string TcpLogSink::recordToJson(const LogRecord& record)
{
    using namespace nlohmann;
    json j;

    j["created"]   = record.created;
    j["filename"]  = record.file;
    j["funcname"]  = record.function;
    j["levelname"] = getLevelString(record.level);
    j["levelno"]   = record.level;
    j["lineno"]    = record.line;
    j["message"]   = record.message;
    j["name"]      = record.name;

    return j.dump();
}