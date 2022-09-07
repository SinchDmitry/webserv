#include "src/ConfigurationSingleton.hpp"
#include "src/Server.hpp"

int main() {
    Server testServer;
    testServer.run();
//    std::map<std::string, std::string> body;
//    body.insert(std::pair<std::string, std::string>("1", "1"));
//    body.insert(std::pair<std::string, std::string>("2", "2"));
//    body.insert_or_assign(std::pair<std::string, std::string>("1", "3"));
//    for(std::map<std::string, std::string>::const_iterator it = body.begin();
//            it != body.end(); ++it)
//    {
//        std::cout << it->first << " " << it->second << "\n";
//    }
    return 0;
}
