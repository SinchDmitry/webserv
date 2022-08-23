#include "LocationInfo.hpp"

LocationInfo::LocationInfo() :
	_indexFile(""),
	_rootDirectory(""),
	_autoindexStatus("off"),
	_metods(0) {}

LocationInfo::LocationInfo(const LocationInfo& copy) {
	*this = copy;
}

LocationInfo&	LocationInfo::operator = (const LocationInfo& op) {
	if (this != &op) {
		_indexFile = op._indexFile;
		_rootDirectory = op._rootDirectory;
		_autoindexStatus = op._autoindexStatus;
		_metods = op._metods;
	}
	return *this;
}

void	LocationInfo::setIndexFile(std::string indexFile) {
	_indexFile = indexFile;
}

void	LocationInfo::setRootDirectory(std::string rootDirectory) {
	_rootDirectory = rootDirectory;
}

void	LocationInfo::setAutoindexStatus(std::string autoindexStatus) {
	_autoindexStatus = autoindexStatus;
}

void	LocationInfo::addMethod(std::string method) {
	_metods.push_back(method);
}

const std::string&	LocationInfo::getIndexFile(void) const {
	return _indexFile;
}
const std::string&	LocationInfo::getRootDirectory(void) const {
	return _rootDirectory;
}

const std::string&	LocationInfo::getAutoindexStatus(void) const {
	return _autoindexStatus;
}

const std::list<std::string>&	LocationInfo::getMetohods(void) const {
	return _metods;
}
