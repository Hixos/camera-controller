/* Copyright (c) 2021 Skyward Experimental Rocketry
 * Author: Luca Erbetta
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

#include "PrintLogger.h"

#include <fmt/chrono.h>
#include <fmt/args.h>

#include <chrono>

using std::condition_variable;
using std::lock_guard;
using std::mutex;
using std::unique_lock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

string getLevelString(uint8_t level)
{
    switch (level)
    {
        case LOGL_DEBUG:
            return "DEBUG";
        case LOGL_EVENT:
            return "EVENT";
        case LOGL_STATE:
            return "STATE";
        case LOGL_INFO:
            return "INFO";
        case LOGL_WARNING:
            return "WARNING";
        case LOGL_ERROR:
            return "ERROR";
        case LOGL_CRITICAL:
            return "CRITICAL";
        default:
            return std::to_string(level);
    }
}

PrintLogger PrintLogger::getChild(const string& name)
{
    return PrintLogger(parent, this->name + "." + name);
}

LogRecord PrintLogger::buildLogRecord(uint8_t level, const string& function,
                                      const string& file, int line,
                                      fmt::string_view format,
                                      fmt::format_args args)
{
    const auto t = high_resolution_clock::now();

    LogRecord record;
    record.created =
        duration_cast<duration<int64_t>>(t.time_since_epoch()).count();
    record.level    = level;
    record.function = function;
    record.file     = file;
    record.line     = line;
    record.name     = name;
    try
    {
        record.message = fmt::vformat(format, args);
    }
    catch (const std::exception& e)
    {
        record.level   = LOGL_ERROR;
        record.message = "FMT Formatting error! " + string(e.what());
    }

    return record;
}

void PrintLogger::vlog(uint8_t level, const string& function,
                       const string& file, int line, fmt::string_view format,
                       fmt::format_args args)
{
    parent.log(buildLogRecord(level, function, file, line, format, args));
}

void PrintLogger::vlogAsync(uint8_t level, const string& function,
                            const string& file, int line,
                            fmt::string_view format, fmt::format_args args)
{
    parent.logAsync(buildLogRecord(level, function, file, line, format, args));
}

void Logging::log(const LogRecord& record)
{
    for (auto& s : sinks)
    {
        if (s->isEnabled())
        {
            s->log(record);
        }
    }
}

void Logging::logAsync(const LogRecord& record) { asyncLog.log(record); }

Logging::AsyncLogger::AsyncLogger(Logging& parent) : parent(parent) {}

void Logging::AsyncLogger::log(const LogRecord& record)
{
    {
        lock_guard<mutex> guard(mutex_log);
        records.put(record);
    }

    cv.notify_all();
}

void Logging::AsyncLogger::run()
{
    while (!shouldStop())
    {
        LogRecord rec;
        {
            unique_lock<mutex> lk(mutex_log);
            cv.wait(lk, [&] { return !records.isEmpty(); });
            rec = records.pop();
        }

        parent.log(rec);
    }
}
