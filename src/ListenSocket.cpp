#include "ListenSocket.hpp"

ListenSocket::ListenSocket(LocationInfo* server) 
	// :
	// _name(server->getConfigList().find("server_name")->second),
	// _port(stoi(*(++(split(server->getConfigList().find("listen")->second, ":").begin())))),
	// _ip(*(split(server->getConfigList().find("listen")->second, ":").begin())),
	// _locations(server->getDownGradeList())
{
	std::multimap<std::string, std::string> mapWithServerInfo = server->getConfigList();
	_name = mapWithServerInfo.find("server_name")->second;
	std::list<std::string> splitForIpPort = split(server->getConfigList().find("listen")->second, ":");
	std::list<std::string>::iterator IpPortIter = splitForIpPort.begin();
	_ip = *(IpPortIter++);
	_port = stoi(*(IpPortIter));
	_locations = server->getDownGradeList();

	/* debug / constructor printer */
	std::cout << this;
}

std::list<std::string> ListenSocket::split(const std::string& str, std::string myDelim) {
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

