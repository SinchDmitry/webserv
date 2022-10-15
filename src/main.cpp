#include "ConfigurationSingleton.hpp"
#include "Server.hpp"
#include "main.hpp"

void ctrl_c_handler(int sig) {
    (void) sig;
    std::cout << "\r" << GRN << "Server was stopped by CTRL+C" << std::endl;
    exit(0);
}

int main() {
    signal(SIGINT, ctrl_c_handler);
    Server testServer;
    testServer.run();
    // Response* response = new Response();
    // std::cout << response->getStatusCodes().find(404)->second << std::endl;
    // delete response;
    return 0;
}
