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

#include "LogSink.h"

#include <string>
#include <memory>
#include <utility>

#include <utils/ActiveObject.h>
#include <sockpp/tcp_connector.h>
#include <utils/collections/SyncCircularBuffer.h>

using std::string;
using std::unique_ptr;
using std::pair;

using BufPtr = unique_ptr<uint8_t[]>;

class TcpLogSink : public LogSink, ActiveObject
{
public:
    TcpLogSink(string ip, uint16_t port);
    ~TcpLogSink();

    void stop() override;

protected:
    void run() override;
    void logImpl(const LogRecord& record) override;
private:
    pair<BufPtr, size_t> packJson(string json);
    string recordToJson(const LogRecord& record);

    string ip;
    uint16_t port;
    sockpp::socket_initializer sockInit;
    sockpp::tcp_connector conn;
    SyncCircularBuffer<LogRecord, 100> records;
    LogRecord curr_record;
    bool curr_record_sent = true;
};