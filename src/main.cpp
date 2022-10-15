#include "ConfigurationSingleton.hpp"
#include "Server.hpp"
#include "main.hpp"

void ctrl_c_handler(int sig) {
    (void) sig;
    std::cout << "\r" << GRN << "Server was stopped by CTRL+C" << std::endl;
    exit(0);
}

std::string timestamp() {
    std::stringstream out;
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    out << END << "[" << std::setfill('0')
              << std::setw(2) << now->tm_hour << ":"
              << std::setw(2) << now->tm_min << ":"
              << std::setw(2) << now->tm_sec << "] ";
    return out.str();
}

void printMsg(int srvNb, std::string msg1, std::string msg2) {
    std::string msg = "";
    if (srvNb >= 0) {
        msg += "server[" + std::to_string(srvNb) + "]: ";
    }
    msg += msg1 + msg2;
    std::cout << timestamp() << YELLOW << msg << END << std::endl;
}

void printMsg(int srvNb, int clntSd, std::string msg1, std::string msg2)
{
    std::string msg = END + timestamp() + YELLOW;
    if (srvNb >= 0)
        msg += "server[" + std::to_string(srvNb) + "]: ";
    msg += msg1;
    clntSd >= 0 ? msg += END + std::to_string(clntSd) + YELLOW : msg += " ";
//    replaceAllStrings(msg2, "\n", "\n\t");
    msg += msg2 + END;
    std::cout << msg << std::endl;
}

int main() {
    signal(SIGINT, ctrl_c_handler);
    printMsg(-1, "Starting server ...", "");
    Server testServer;
    testServer.run();
    // Response* response = new Response();
    // std::cout << response->getStatusCodes().find(404)->second << std::endl;
    // delete response;
    return 0;
}
