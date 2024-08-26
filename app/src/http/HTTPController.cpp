#include "HTTPController.hpp"

const HTTPParser* HTTPController::httpParser = nullptr;

HTTPController::HTTPController(Server* server, AvailableHTTPProtocols protocol):
server(server) {
    httpParser = HTTPParser::getHTTPParser(protocol);
}