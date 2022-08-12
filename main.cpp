#include "Server.hpp"

int main() {
    Server testServer;
    for (int i = 0; i < 5; ++i) {
        testServer.createTestListSockets();
    }
    
}