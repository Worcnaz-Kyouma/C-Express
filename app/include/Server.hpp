#ifndef CE_SERVER_H
#define CE_SERVER_H

#include <thread>
#include <vector>
#include <string>
#include <memory>

#include "Types.hpp"
#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Server {
private:
    Socket* serverSocket;
    std::vector<std::unique_ptr<std::thread>> serverThreads;

    const HTTPController* httpController;

    void serveRequest(Socket* clientSocket);
    void processRequest(Socket* clientSocket);

    void sendResponse(Socket* clientSocket, const Response& res);
public:
    Server();
    
    void get(std::string rawEndpoint, void (*handler)(Request, Response));
    void post(std::string rawEndpoint, void (*handler)(Request, Response));

    void listen(unsigned int port);
    
    // Test methods
    void promptResources(); 
};

#endif