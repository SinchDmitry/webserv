#include "Server.hpp"

int main() {
    Server testServer;
    int socketId = testServer.initListningSocket();
    if (socketId != -1) {
        testServer.run(socketId);
    }
}