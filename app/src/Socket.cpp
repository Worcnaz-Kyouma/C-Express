#include "Socket.hpp"
#include "SocketUnix.hpp"

Socket* Socket::getSocket() {
    Socket* unixSocket = new SocketUnix();

    return unixSocket;
}