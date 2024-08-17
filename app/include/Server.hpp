#ifndef CE_SERVER_H
#define CE_SERVER_H

#include <thread>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include "Types.hpp"
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
    std::vector<std::unique_ptr<std::thread>> serverThreads;
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
    void get(std::string rawEndpoint, void (*handler)(Request, Response));
    void post(std::string rawEndpoint, void (*handler)(Request, Response));
    void patch(std::string rawEndpoint, void (*handler)(Request, Response));
    void remove(std::string rawEndpoint, void (*handler)(Request, Response));
    
    // Test methods
    void promptResources(); 
};

#endif