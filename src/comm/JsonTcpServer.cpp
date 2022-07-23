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

#include "JsonTcpServer.h"

#include <arpa/inet.h>

#include <chrono>
#include <csignal>
#include <cstddef>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <thread>

using std::lock_guard;
using std::make_pair;
using std::make_unique;
using std::unique_lock;
using std::unique_ptr;
using std::chrono::seconds;
using std::this_thread::sleep_for;

JsonTcpServer::JsonTcpServer(uint16_t port, JsonTcpServer::ReceiverFun fun)
    : fun(fun), acc(port)
{
    if (!acc)
    {
        throw TcpAcceptorError(acc.last_error_str());
    }

    LOG_INFO(log, "Created acceptor on address {}", acc.address().to_string());

    start();
}

JsonTcpServer::~JsonTcpServer() { stop(); }

void JsonTcpServer::stop()
{
    if (started && !stopped)
    {
        should_stop = true;
        stopSender();

        if (thread_obj->joinable())
            thread_obj->join();
        stopped = true;
    }
}

bool JsonTcpServer::isConnected()
{
    // lock_guard<mutex> lock(mtx_parent);
    return is_connected;
}

void JsonTcpServer::send(json&& j)
{
    // LOG_DEBUG(log, "Sending json packet: {}", j.dump(-1));
    buf_out.put(std::move(j));
}

void JsonTcpServer::send(const json& j)
{
    // LOG_DEBUG(log, "Sending json packet: {}", j.dump(-1));
    buf_out.put(j);
}

void JsonTcpServer::run()
{
    while (!shouldStop())
    {
        sockpp::inet_address peer;
        sockpp::tcp_socket sock = acc.accept(&peer);
        if (!sock)
        {
            LOG_ERR(log, "Invalid socket! what: {}", acc.last_error_str());
            sleep_for(seconds(2));
            continue;
        }
        LOG_DEBUG(log, "Accepted connection from: {}", peer.to_string());
        is_connected = true;

        auto sock_rcv = sock.clone();
        unique_ptr<Receiver> receiver =
            make_unique<Receiver>(std::move(sock_rcv), *this);

        while (is_connected)
        {
            if (last_packet.size() == 0)
            {
                json j = buf_out.popBlocking();

                // Empty json, stop sender
                if (j.is_null())
                    break;
                else
                    last_packet = std::move(pack(j));
            }

            int res = sock.write_n(last_packet.data(), last_packet.size());
            if (res < 0 || (size_t)res != last_packet.size())
            {
                if (sock.last_error() == EPIPE)
                {
                    LOG_INFO(log, "{} disconnected", peer.to_string());
                    break;
                }
                else
                {
                    LOG_ERR(log,
                            "Error sending packet! (size: {}, res: {}, err: "
                            "{}, msg: {})",
                            last_packet.size(), res, sock.last_error_str(),
                            sock.last_error_str());
                    break;
                }
            }
            else
            {
                // Successfully sent, clear the vector
                last_packet.clear();
            }
        }

        is_connected = false;
        receiver->stop();
        LOG_DEBUG(log, "Receiver stopped!");
    }
}

vector<uint8_t> JsonTcpServer::pack(const json& j)
{
    string str = j.dump();
    vector<uint8_t> v;
    v.resize(str.length() + 4);

    uint32_t len = htonl(str.length());
    memcpy(v.data(), &len, 4);
    memcpy(v.data() + 4, str.data(), str.length());

    return v;
}

void JsonTcpServer::stopSender()
{
    is_connected = false;
    buf_out.put({});
}

JsonTcpServer::Receiver::Receiver(sockpp::tcp_socket&& sock,
                                  JsonTcpServer& parent)
    : sock(std::move(sock)), parent(parent),
      log(parent.log.getChild("Rcv"))
{
    start();
}

JsonTcpServer::Receiver::~Receiver() { stop(); }

void JsonTcpServer::Receiver::stop()
{
    if (started && !stopped)
    {
        should_stop = true;
        sock.shutdown();

        if (thread_obj->joinable())
            thread_obj->join();
        stopped = true;
    }
}

void JsonTcpServer::Receiver::run()
{
    while (!shouldStop())
    {
        uint8_t len_arr[4];
        ssize_t res = sock.read_n(len_arr, 4);
        if (res == 0)
        {
            LOG_INFO(log, "{} disconnected", sock.peer_address().to_string());
            break;
        }
        else if (res != 4)
        {
            LOG_ERR(log,
                    "Error reading packet size! (res: {}, err: {}, msg: {})",
                    res, sock.last_error(), sock.last_error_str());
            break;
        }

        uint32_t len;
        memcpy(&len, len_arr, 4);
        len = ntohl(len);

        vector<uint8_t> v;
        v.resize(len);

        res = sock.read_n(v.data(), len);
        if (res == 0)
        {
            LOG_INFO(log, "{} disconnected", sock.peer_address().to_string());
            break;
        }
        else if (res < 0 || (size_t)res != len)
        {
            LOG_ERR(log, "Error reading packet! (res: {}, err: {}, msg: {})",
                    res, sock.last_error(), sock.last_error_str());
            break;
        }

        parent.fun(json::parse(v));
    }

    sock.shutdown();
    parent.stopSender();
}