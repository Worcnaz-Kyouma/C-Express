#ifndef CE_SERVER_H
#define CE_SERVER_H

#include <thread>
#include <vector>
#include <string>
#include <memory>

#include "Types.hpp"
#include "socket/Socket.hpp"
class HTTPController;
#include "http/HTTPController.hpp"
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
    
    void get(std::string rawEndpoint, ResourceManager);
    void post(std::string rawEndpoint, ResourceManager);

    void listen(unsigned int port);
    friend class HTTPController;
};

#endif