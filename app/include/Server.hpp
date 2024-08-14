#ifndef CE_SERVER_H
#define CE_SERVER_H

#include <thread>
#include <vector>
#include <map>
#include <string>
#include <memory>

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
using Endpoint = std::vector<std::string>;

class Server {
private:
    Socket* serverSocket;
    std::vector<std::thread> serverThreads;
    const static HTTPParser httpParser;

    std::map<Endpoint, Process> resources;

    void serveRequest(Socket* clientSocket);
    void processRequest(Socket* clientSocket);
    std::map<Endpoint, Process>::iterator findResource(Request);
    std::vector<Endpoint> getResourceEndpoint(Endpoint targetEndpoint);
public:
    Server();

    void listen(unsigned int port);
    void addResource(AvailableMethods method, std::string rawEndpoint, void (*handler)(Request, Response));
    void promptResources();
};

#endif