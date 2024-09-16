#ifndef CE_SERVER_H
#define CE_SERVER_H

#include <thread>
#include <vector>
#include <string>
#include <memory>

#include "Types.hpp"
#include "Socket.hpp"
#include "HTTPController.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Server {
private:
    std::vector<std::unique_ptr<std::thread>> serverThreads;
    
    Socket* const serverSocket;
    HTTPController* const httpController;

    void serveRequest(Socket* clientSocket);
    void processRequest(Socket* clientSocket);

    void sendResponse(const std::string& response, const Socket* clientSocket);
public:
    Server();
    explicit Server(AvailableHTTPProtocols protocol);
    ~Server();
    
    void get(std::string rawEndpoint, void (*handler)(Request, Response));
    void post(std::string rawEndpoint, void (*handler)(Request, Response));

    void listen(unsigned int port);
    
    // Test methods
    void promptResources(); 

    friend class HTTPController;
};

#endif