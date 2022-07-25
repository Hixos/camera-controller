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

#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "LogSink.h"

using std::string;

class JsonLogSink : public BaseFileLogSink
{
public:
    JsonLogSink(string file) : BaseFileLogSink(file)
    {
        string b = "[\n";
        fwrite(b.c_str(), sizeof(char), b.length(), f);
    }

    ~JsonLogSink()
    {
        string b = "]";
        fwrite(b.c_str(), sizeof(char), b.length(), f);
    }

protected:
    string recordToString(const LogRecord& record) override
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

        if (first)
        {
            return j.dump() + "\n";
        }
        else
        {
            return "," + j.dump() + "\n";
        }
    }

private:
    bool first = true;
};