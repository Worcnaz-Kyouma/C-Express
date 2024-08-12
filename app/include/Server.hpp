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

class Server {
private:
    Socket* serverSocket;
    std::vector<std::thread> serverThreads;
    const static HTTPParser httpParser();

    std::map<char**, void (*)(Request, Response)> resources;

    void serveRequest(Socket*);
    void processRequest(Socket*);
public:
    Server();

    void listen(unsigned int port);
    void addResource(AvailableMethods method, std::string endpoint, void (*)(Request, Response));
};

#endif