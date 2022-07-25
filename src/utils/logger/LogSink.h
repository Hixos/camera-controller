/* Copyright (c) 2021 Skyward Experimental Rocketry
 * Authors: Luca Erbetta, Luca Conterio
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

#pragma once

#include <mutex>
#include <string>

#include "PrintLoggerData.h"

using std::string;
using std::mutex;

class LogSink
{
public:
    LogSink() {}
    LogSink(const LogSink&) = delete;
    LogSink& operator=(const LogSink&) = delete;

    virtual ~LogSink() {}

    void log(const LogRecord& record);

    void enable() { enabled = true; }

    void disable() { enabled = false; }

    bool isEnabled() { return enabled; }

    void setLevel(uint8_t level) { minimumLevel = level; }

    int getLevel() { return minimumLevel; }

protected:
    virtual void logImpl(const LogRecord& record) = 0;

private:
    bool enabled         = true;  // enabled by the default when created
    uint8_t minimumLevel = LOGL_NOTSET;
};

/**
 * This class directly outputs the log to file.
 */
class StdoutLogSink : public LogSink
{
public:
    StdoutLogSink() {}

    void setFormatString(const std::string& format) { this->format = format; }

protected:
    void logImpl(const LogRecord& record) override;

    FILE* f;
    mutex mutex_file;

private:
    std::string format = "{:%H:%M:%S} {lvl:^8} {name:>15} > {msg}\n";
};


class BaseFileLogSink : public LogSink
{
public:
    BaseFileLogSink(string file);
    virtual ~BaseFileLogSink();

protected:
    virtual string recordToString(const LogRecord& record) = 0;
    void logImpl(const LogRecord& record) override;

    FILE* f;
    mutex mutex_file;
};

/**
 * This class directly outputs the log to file.
 */
class FileLogSink : public BaseFileLogSink
{
public:
    FileLogSink(string file);
    ~FileLogSink() = default;

    void setFormatString(const std::string& format) { this->format = format; }

protected:
    string recordToString(const LogRecord& record) override;

private:
    std::string format = "{::%Y-%m-%d %H:%M:%S} {file}:{line} {fun} {lvl}\t[{name}]\t{msg}\n";
};