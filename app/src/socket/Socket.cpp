#include "socket/Socket.hpp"
#include "socket/SocketUnix.hpp"

Socket* Socket::getSocket() {
    Socket* unixSocket = new SocketUnix();

    return unixSocket;
}