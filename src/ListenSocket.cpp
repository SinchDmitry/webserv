#include "ListenSocket.hpp"

//* debug function / recource printer
void recoursePrinter(LocationInfo* locationInf) {
    std::cout << "-==GENERAL==-" << std::endl;
    std::cout << "type : " << locationInf->getType() << std::endl;
    std::cout << "lctn : " << locationInf->getLocation() << std::endl;
    std::multimap<std::string, std::string> config = locationInf->getConfigList();
    if (!config.empty()) {
        std::cout << "-==DATA==-" << std::endl;
        for (std::multimap<std::string, std::string>::iterator configIter = config.begin();
             configIter != config.end(); ++configIter) {
            std::cout << "parameter : " << configIter->first <<
                      " | value : " << configIter->second << std::endl;
        }
    }
    std::list<LocationInfo*> location = locationInf->getDownGradeList();
    // std::cout << "size : " << location.size() << std::endl;
    if (!location.empty()) {
        for (std::list<LocationInfo*>::iterator locationIter = location.begin();
             locationIter != location.end(); ++locationIter) {
            std::cout << "-==INCLUDE==-" << std::endl;
            recoursePrinter(*locationIter);
        }
    }
    std::cout << "-==END of " << locationInf->getType() << " ==-" << std::endl;
}

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
	 std::cout << *this;

//    recoursePrinter(server);
}
ListenSocket&	ListenSocket::operator = (const ListenSocket& op) {
	if (this != &op) {
		_port = op._port;
		_ip = op._ip;
		_name = op._name;
		_locations = op._locations;
	}
	return *this;
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

