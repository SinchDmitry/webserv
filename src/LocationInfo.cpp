#include "LocationInfo.hpp"

LocationInfo::LocationInfo() : _type("") {}

LocationInfo::LocationInfo(const LocationInfo& copy) { *this = copy; }

LocationInfo&	LocationInfo::operator = (const LocationInfo& op) {
	if (this != &op) {
		_type = op._type;
		_config = op._config;
	}
	return *this;
}

void	LocationInfo::configPushBack(std::string key, std::string value) {
	_config.insert(key, value);
}