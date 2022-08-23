#include "ConfigurationSingleton.hpp"

ConfigurationSingleton::ConfigurationSingleton() {
	fileInit();
}

ConfigurationSingleton::ConfigurationSingleton(const ConfigurationSingleton& copy) {
	*this = copy;
}

ConfigurationSingleton& ConfigurationSingleton::operator = (const ConfigurationSingleton& op) {
	if (this != &op) {
		_port = op._port;
		_ipAddress = op._ipAddress;
		_instance = op._instance;
		_locationData = op._locationData;
	}
	return *this;
}

ConfigurationSingleton* ConfigurationSingleton::_instance = 0;

ConfigurationSingleton*	ConfigurationSingleton::getInstance(void) {
	if (!_instance) {
		_instance = new ConfigurationSingleton();
	}
	return _instance;
}

std::vector<std::string> ConfigurationSingleton::split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
	std::vector<std::string> elems;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void	ConfigurationSingleton::fileInit() {
	std::ifstream file;
	file.open("configuration.conf", std::ios::in | std::ios::ate);
	if (file.fail()) {
		perror("Error : can't open input file");
		exit(1);
	}
	int size = file.tellg();
	char buffer[size];
	file.seekg(0);
	file.read(buffer, size);

	std::vector<std::string> configInfo = split(buffer, '\n');
	for (int i = 0; i < configInfo.size(); ++i) {
		std::cout << configInfo[i] << std::endl;
	}
}
