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

#include "LogSink.h"

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include <cerrno>
#include <stdexcept>

using std::lock_guard;
using std::mutex;

void LogSink::log(const LogRecord& record)
{
    if (record.level >= minimumLevel)
    {
        logImpl(record);
    }
}

BaseFileLogSink::BaseFileLogSink(string file)
{
    f = fopen(file.c_str(), "a");
    if (!f)
        throw std::system_error(errno, std::generic_category(),
                                "Cannot open file " + file);
}

BaseFileLogSink::~BaseFileLogSink()
{
    if (f)
        fclose(f);
}

void BaseFileLogSink::logImpl(const LogRecord& record)
{
    string r = recordToString(record);
    {
        lock_guard<mutex> guard(mutex_file);
        fwrite(r.c_str(), sizeof(char), r.length(), f);
    }
}

FileLogSink::FileLogSink(string file) : BaseFileLogSink(file)
{
}


string FileLogSink::recordToString(const LogRecord& record)
{
    using namespace fmt::literals;

    string l = fmt::format(format, fmt::localtime(record.created),
                           "file"_a = record.file, "line"_a = record.line,
                           "fun"_a  = record.function,
                           "lvl"_a  = getLevelString(record.level),
                           "name"_a = record.name, "msg"_a = record.message);
    
    return l;
}

void StdoutLogSink::logImpl(const LogRecord& record)
{
    using namespace fmt::literals;
    // const auto tsStr =
    //     fmt::format("{::%Y-%m-%d %H:%M:%S}", fmt::localtime(record.created));

    lock_guard<mutex> guard(mutex_file);
    fmt::text_style color;
    if (record.level >= LOGL_CRITICAL)
    {
        color = bg(fmt::color::red) | fg(fmt::color::white);
    }
    else if (record.level >= LOGL_ERROR)
    {
        color = fg(fmt::color::red);
    }
    else if (record.level >= LOGL_WARNING)
    {
        color = fg(fmt::color::orange);
    }
    else if (record.level >= LOGL_INFO)
    {
        color = fg(fmt::color::light_blue);
    }
    else if (record.level >= LOGL_EVENT)
    {
        color = fg(fmt::color::light_gray);
    }
    else
    {
        color = fg(fmt::color::gray);
    }
    fmt::print(color, format, fmt::localtime(record.created),
               "file"_a = record.file, "line"_a = record.line,
               "fun"_a = record.function,
               "lvl"_a = getLevelString(record.level), "name"_a = record.name,
               "msg"_a = record.message);
    // string l = fmt::format("{} {} {}", getLevelString(record.level),
    // record.name, record.message);
}