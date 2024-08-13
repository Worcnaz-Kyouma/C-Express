#ifndef CE_SERVER_H
#define CE_SERVER_H

#include <thread>
#include <vector>
#include <map>
#include <string>

#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "HTTPParser.hpp"

enum AvailableMethods {
    GET,
    POST,
    PATCH,
    DELETE
};

using Process = void(*)(Request, Response);

class Server {
private:
    Socket* serverSocket;
    std::vector<std::thread> serverThreads;
    const static HTTPParser httpParser;

    std::map<std::vector<std::string>, Process> resources;

    void serveRequest(Socket* clientSocket);
    void processRequest(Socket* clientSocket);
    Process findProcess(Request);
public:
    Server();

    void listen(unsigned int port);
    void addResource(AvailableMethods method, std::string endpoint, void (*)(Request, Response));
};

#endif