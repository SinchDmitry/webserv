#include "ConfigurationSingleton.hpp"
#include "Server.hpp"
#include "main.hpp"

std::string replace(std::string src, std::string s1, std::string s2) {
    std::string dest = "";
    size_t index;
    if (s1 == s2) { return src; }
    index = src.find(s1);
    while (index != std::string::npos) {
        dest = src.substr(0, index) + s2 + src.substr(index + s1.length());
        src = dest;
        index = src.find(s1);
    }
    return dest;
}

void ctrl_c_handler(int sig) {
    (void) sig;
    std::cout << "\r" << GRN << "Server was stopped by CTRL+C" << std::endl;
    exit(0);
}

std::list<std::string> split(const std::string& str, std::string myDelim) {
    std::list<std::string> dest;
    char* delim = (char *)myDelim.c_str();
    char* pTempStr = strdup(str.c_str());
    char* pWord = strtok(pTempStr, delim);
    while(pWord != NULL) {
        dest.push_back(pWord);
        pWord = strtok(NULL, delim);
    }
    free(pTempStr);
    return dest;
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
//    msg2 = replace(msg2, "\n", "\n\t");
    msg += msg2 + END;
    std::cout << msg << std::endl;
}

void printMsg(int srvNb, int clntSd, std::string COLOR, std::string msg1, std::string msg2)
{
    std::string msg = END + timestamp() + YELLOW;
    if (srvNb >= 0)
        msg += "server[" + std::to_string(srvNb) + "]: ";
    msg += END + COLOR + msg1;
    clntSd >= 0 ? msg += END + std::to_string(clntSd) + COLOR : msg += " ";
//    msg2 = replace(msg2, "\n", "\n\t");
    msg += msg2 + END;
    std::cout << msg << std::endl;
}

void printValue(std::string key, std::string value) {
    std::cout << BLUE << key << " : " << END << value << std::endl;
}

int main() {
    signal(SIGINT, ctrl_c_handler);
    printMsg(-1, "Starting server ...", "");
    Server testServer;
    testServer.run();
    return 0;
}
