#ifndef CE_SERVER_H
#define CE_SERVER_H

#include <thread>
#include <vector>
#include "Socket.hpp"

class Server {
private:
    Socket* serverSocket;
    std::vector<std::thread> serverThreads;
public:
    Server();

    void listen();
    void addResource();
};

#endif