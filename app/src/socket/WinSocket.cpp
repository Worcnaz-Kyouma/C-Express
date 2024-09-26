#include "socket/WinSocket.hpp"
#include <stdexcept>

WinSocket::WinSocket() {
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        throw std::runtime_error("Error starting up the dynamic lib(i think is that lol, didnt study winsocket enought)");
    }

    struct addressInfo *addrInfo = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Nah bro, still need to finish it
}