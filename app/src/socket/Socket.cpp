#include "socket/Socket.hpp"
#ifdef _WIN32
    #include "socket/WinSocket.hpp";
#else __linux__
    #include "socket/SocketUnix.hpp";
#endif

Socket* Socket::getSocket() {
    Socket* socket = nullptr;

    #ifdef _WIN32
        socket = new WinSocket();
    #else __linux__
        socket = new SocketUnix();
    #endif

    return socket;
}