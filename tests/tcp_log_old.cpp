#include <errno.h>
#include <fmt/core.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <utility>

using namespace std;
using namespace nlohmann;
using fmt::print;
using std::pair;

string makeJsonLog()
{
    json j;
    j["created"]   = 1657924485.4800348;
    j["filename"]  = "prova.txt";
    j["funcname"]  = "prova()";
    j["levelname"] = "ERROR";
    j["levelno"]   = 50;
    j["lineno"]    = 123;
    j["message"]   = "this is a message";
    j["name"]      = "test";
    return j.dump();
}

pair<uint8_t*, size_t> packLog(string logrecord)
{
    uint32_t len   = logrecord.length();
    uint8_t* buf   = new uint8_t[len + 4];
    uint32_t len_n = htonl(len);

    memcpy(buf, &len_n, sizeof(len_n));
    memcpy(buf + 4, logrecord.data(), logrecord.length());
    return {buf, logrecord.length() + 4};
}

class TcpClient
{
public:
    TcpClient() : connected(false) {}

    bool tcpconnect(string ip, string port)
    {
        if (connected)
            return false;

        print("Connecting\n");
        struct addrinfo hints;
        struct addrinfo* server_info;

        hints.ai_family   = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags    = AI_NUMERICHOST;

        int res = getaddrinfo(ip.c_str(), port.c_str(), &hints, &server_info);

        if (res != 0)
        {
            print("Get addrinfo failed: {} ({})\n", res, gai_strerror(res));
            // freeaddrinfo(server_info);
            return false;
        }

        sock_fd = socket(server_info->ai_family, server_info->ai_socktype,
                         server_info->ai_protocol);

        if (sock_fd < 0)
        {
            print("Failed to create socket: {} ({})\n", errno, strerror(errno));
            // freeaddrinfo(server_info);
            return false;
        }
        else
        {
            print("Created socket! {}\n", sock_fd);
        }

        // res = bind(sock_fd, server_info->ai_addr, server_info->ai_addrlen);
        // if(res != 0)
        // {
        //     freeaddrinfo(server_info);
        //     close(sock_fd);
        //     print("Failed to create socket: {} ({})\n", errno,
        //     strerror(errno));
        // }

        res = connect(sock_fd, server_info->ai_addr, server_info->ai_addrlen);
        if (sock_fd < 0)
        {
            print("Failed to connect: {} ({})\n", errno, strerror(errno));
            // freeaddrinfo(server_info);
            close(sock_fd);
            return false;
        }
        connected = true;

        // freeaddrinfo(server_info);
        return true;
    }

    bool tcpsend(uint8_t* data, size_t len)
    {
        if (!connected)
            return false;

        int sent = 0;
        while (sent < len)
        {
            int res = send(sock_fd, data + sent, len - sent, 0);
            if (res < 0)
            {
                print("Failed to send: {} ({})\n", errno, strerror(errno));
                return false;
            }
            else
            {
                sent += res;
            }
        }
        return true;
    }

    bool tcpdisconnect()
    {
        if (connected)
        {
            connected = false;
            close(sock_fd);
            sock_fd = -1;
            return true;
        }
        return false;
    }

private:
    int sock_fd;
    int port;
    bool connected;
};

int main()
{
    TcpClient client{};

    uint8_t* data;
    size_t len;

    auto val = packLog(makeJsonLog());
    data     = val.first;
    len      = val.second;

    client.tcpconnect("192.168.1.102", "19996");

    for (;;)
    {
        client.tcpsend(data, len);
        print("Log sent!\n");
        this_thread::sleep_for(chrono::seconds(3));
    }
}