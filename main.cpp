#include "src/ConfigurationSingleton.hpp"
#include "src/Server.hpp"

int main() {
   Server testServer;
   testServer.run();
    // Response* response = new Response();
    // std::cout << response->getStatusCodes().find(404)->second << std::endl;
    // delete response;
    return 0;
}
