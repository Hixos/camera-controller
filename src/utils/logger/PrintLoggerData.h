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

#include <string>
#include <cstdint>

using std::string;

struct LogRecord
{
    int level;
    double created;
    std::string function;
    std::string file;
    int line;
    std::string name;
    std::string message;
};

enum LogLevel : uint8_t
{
    LOGL_NOTSET   = 0,
    LOGL_EVENT    = 5,
    LOGL_DEBUG    = 10,
    LOGL_STATE    = 15,
    LOGL_INFO     = 20,
    LOGL_WARNING  = 30,
    LOGL_ERROR    = 40,
    LOGL_CRITICAL = 50
};

string getLevelString(uint8_t level);
