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

#include "UDPEchoServer.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>

UDPEchoServer::UDPEchoServer(string address, uint16_t port, bool is_multicast)
    : sockInit(), sock()
{
    sockpp::inet_address addr{address, port};

    if (!sock.bind(addr))
    {
        LOG_ERR(log, "Cannot bind to {}:{}", address, port);
        throw std::runtime_error(
            fmt::format("Cannot bind to {}:{}", address, port));
    }

    if (is_multicast)
    {
        addrinfo hints;
        memset(&hints, 0, sizeof(hints));

        hints.ai_family   = AF_INET;  // IPv4, same as the provided multicast ip
        hints.ai_socktype = SOCK_DGRAM;  // UDP
        hints.ai_flags    = AI_NUMERICHOST;

        // Get addrinfo of the multicast group we want to bind to
        addrinfo* group_info;
        int res = getaddrinfo(address.c_str(), fmt::format("{}", port).c_str(),
                              &hints, &group_info);
        if (res < 0)
        {
            LOG_ERR(log, "Getaddrinfo error {}} ({}})", res,
                    gai_strerror(res));
            throw std::runtime_error(fmt::format("getaddrinfo error", address));
        }
        int sock_fd = sock.handle();

        sockaddr_in*  group_addr = (sockaddr_in*)group_info->ai_addr;

        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = group_addr->sin_addr.s_addr;
        mreq.imr_interface.s_addr =
            htonl(INADDR_ANY);  // Receive on any interface

        if (setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                       (char*)&mreq, sizeof(mreq)) < 0)
        {
            LOG_ERR(log, "Cannot join multicast group {}: {}", address,
                    strerror(errno));
            throw std::runtime_error(
                fmt::format("Cannot join multicast group {}", address));
        }
    }
    start();
}

UDPEchoServer::~UDPEchoServer() { stop(); }

void UDPEchoServer::stop()
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

void UDPEchoServer::run()
{
    while (!shouldStop())
    {
        sockpp::inet_address peer;
        uint8_t data;
        int n = sock.recv_from(&data, 1, &peer);
        LOG_DEBUG(log, "Received UDP packet from {}. content: {{{}}}", peer.to_string(), data);

        if (n == 1 && data == 0x66)
        {
            data = 0x77;
            sock.send_to(&data, 1, peer);
            LOG_DEBUG(log, "Respondend to {}.", peer.to_string());
        }
        else if (sock.last_error() == EPIPE)
        {
            LOG_INFO(log, "UDP socket closed.");
        }
        else
        {
            LOG_ERR(log, "Error receiving from socket");
            break;
        }
    }
}