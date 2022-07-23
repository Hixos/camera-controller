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

#include <sockpp/tcp_acceptor.h>
#include <nlohmann/json.hpp>

#include <string>
#include <exception>
#include <functional>
#include <utility>
#include <vector>

#include "utils/ActiveObject.h"
#include "utils/collections/SyncCircularBuffer.h"
#include "PrintLogger.h"

using std::function;
using std::vector;
using nlohmann::json;

class JsonTcpServer : public ActiveObject
{  
public:
    using ReceiverFun = function<void(const json&)>;

    JsonTcpServer(uint16_t port, ReceiverFun fun);

    JsonTcpServer(const JsonTcpServer& other) = delete;
    JsonTcpServer(JsonTcpServer&& other) = delete;

    JsonTcpServer& operator=(const JsonTcpServer& rhs) = delete;
    JsonTcpServer& operator=(JsonTcpServer&& rhs) = delete;

    ~JsonTcpServer();

    bool isConnected();

    void stop();

    void send(const json& j);
    void send(json&& j);

    struct TcpAcceptorError : public std::exception
    { 
        TcpAcceptorError(std::string wh) : wh(wh) {}

        const char* what() const noexcept override { return wh.c_str(); }

    private:
        std::string wh;
    };

protected:
    void run() override;

private:
    vector<uint8_t> pack(const json& j);

    void stopSender();

    class Receiver : public ActiveObject
    {
    public:
        Receiver(sockpp::tcp_socket&& sock, JsonTcpServer& parent);
        ~Receiver();

        void stop() override;
    protected:
        void run();

    private:

        sockpp::tcp_socket sock;
        JsonTcpServer& parent;
        PrintLogger log;
    };

    static constexpr unsigned int BUFFER_SIZE = 1000;   
    SyncCircularBuffer<json, BUFFER_SIZE> buf_out{};

    ReceiverFun fun;

    sockpp::tcp_acceptor acc;

    atomic_bool is_connected = false;
    vector<uint8_t> last_packet{};

    PrintLogger log = Logging::getLogger("TcpServ");

    sockpp::socket_initializer sockInit;
};
