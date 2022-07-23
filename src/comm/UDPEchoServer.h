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

#include <string>
#include <sockpp/udp_socket.h>

#include "utils/ActiveObject.h"
#include "PrintLogger.h"

using std::string;

class UDPEchoServer : public ActiveObject
{
public:
    UDPEchoServer(string address, uint16_t port, bool is_multicast);
    ~UDPEchoServer();

    UDPEchoServer(const UDPEchoServer& other) = delete;
    UDPEchoServer(UDPEchoServer&& other) = delete;
    UDPEchoServer& operator=(const UDPEchoServer& other) = delete;
    UDPEchoServer& operator=(UDPEchoServer&& other) = delete;

    void stop() override;
protected:
    void run() override;

private:
    sockpp::socket_initializer sockInit;
    sockpp::udp_socket sock;

    PrintLogger log = Logging::getLogger("UDPEchoServer");
};
