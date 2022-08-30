#include "ConfigurationSingleton.hpp"

ConfigurationSingleton::ConfigurationSingleton() : _tree(NULL) {
	fileParse(fileInit());
}

ConfigurationSingleton*	ConfigurationSingleton::getInstance(void) {
	static ConfigurationSingleton* 	_instance;
	if (!_instance) {
		_instance = new ConfigurationSingleton();
	}
	return _instance;
}

std::list<std::string> ConfigurationSingleton::split(const std::string& s, char delim) {
    std::stringstream ss(s);
    std::string item;
	std::list<std::string> elems;
    while(std::getline(ss, item, delim)) {
        if (item != "") {
			elems.push_back(item);
		}
    }
    return elems;
}

std::list<std::string>	ConfigurationSingleton::fileInit() {
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

	std::list<std::string> configInfo = split(buffer, '\n');
	return configInfo;
}

LocationInfo ConfigurationSingleton::downgradeConfigList(std::list<std::string>::iterator* a) {
	LocationInfo downGrade;
	std::list<std::string> infoString = split(**a, ' ');
	// std::cout << "here : "<< **a << " size : " << infoString.size() << std::endl;
	if (infoString.size() == 2) {
		downGrade.setType(*infoString.begin());
	} else if (infoString.size() == 3) {
		std::list<std::string>::iterator tmpIter = infoString.begin()++;
		downGrade.setType(*infoString.begin());
		downGrade.setLocation(*tmpIter);
	} else {
		exit(1);
	}
	// std::cout << "!!! scope info " << *infoString.begin() << std::endl;
	while (true) {
		++(*a);
		
		infoString = split(**a, ' ');
		// std::cout << "!!! string info : " << **a << " length : " << infoString.size() << std::endl;
		// std::cout << "	!!! end string info : " << *(--infoString.end()) << std::endl;
		// std::cout << "	!!! begin string info : " << *infoString.begin() << std::endl;
		if (*infoString.begin() == "#") {
			continue;
		}
		// std::cout << "here : "<< **a << " size : " << infoString.size() << std::endl;
		if {}
		if (*(--infoString.end()) == "{") {
			std::cout << "here open : "<< **a << " size : " << infoString.size() << std::endl;
			downGrade.configListPushBack(downgradeConfigList(a));
			std::cout << "here close : "<< **a << " size : " << infoString.size() << std::endl;
		} 
		if (*(--infoString.end()) == "}") {
			// std::cout << "here close : "<< **a << " size : " << infoString.size() << std::endl;
			break;
		} 
		if (infoString.size() == 2) {
			std::list<std::string>::iterator tmpIter = infoString.begin()++;
			downGrade.configMapPushBack(*infoString.begin(), *tmpIter);
		}
	}
	return downGrade;
}

void	ConfigurationSingleton::fileParse(std::list<std::string> inputFile) {
	std::list<std::string>::iterator a = inputFile.begin(); // создал итератор на начало
	LocationInfo* treeHead = new LocationInfo("root", "base"); // создал объект в котором будет храниться голова дерева
	while (a != inputFile.end()) { // парсинг файла до последней строчки
		std::cout << "on start : " << *a << std::endl;
		std::list<std::string> infoString = split(*a, ' ');
		if (*a != "" && *(--infoString.end()) == "{") {
			treeHead->configListPushBack(downgradeConfigList(&a)); // рекурсивная функция
		}
		std::cout << "at end : " << *a << std::endl;
		++a;
	}
	_tree = treeHead; // голову списка сохраняем, как голову дерева
}
