#include "socket/Socket.hpp"
#include "socket/Unix.hpp"

Socket* Socket::getSocket() {
    Socket* unixSocket = new SocketUnix();

    return unixSocket;
}