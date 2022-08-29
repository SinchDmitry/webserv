#include "ConfigurationSingleton.hpp"

ConfigurationSingleton::ConfigurationSingleton() : _tree(NULL) {
	fileParse(fileInit());
}

ConfigurationSingleton::ConfigurationSingleton(const ConfigurationSingleton& copy) {
	*this = copy;
}

ConfigurationSingleton& ConfigurationSingleton::operator = (const ConfigurationSingleton& op) {
	if (this != &op) {
		_tree = op._tree;
		_instance = op._instance;
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

std::list<std::string> ConfigurationSingleton::split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
	std::list<std::string> elems;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
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
	for (std::list<std::string>::iterator a = configInfo.begin(); a != configInfo.end(); ++a) {
		std::cout << *a << std::endl;
	}
	return configInfo;
}

LocationInfo ConfigurationSingleton::downgradeConfigList(std::list<std::string>::iterator a) {
	LocationInfo downGrade;
	std::list<std::string> infoString = split(*a, ' ');
	if (infoString.size() == 2) {
		downGrade.setType(*infoString.begin());
	} else if (infoString.size() == 3) {
		std::list<std::string>::iterator tmpIter = infoString.begin()++;
		downGrade.setType(*infoString.begin());
		downGrade.setLocation(*tmpIter);
	} else {
		exit(1);
	}
	while (true) {
		++a;
		infoString = split(*a, ' ');
		if (*infoString.end() == "}") {
			break ;
		} else if (*infoString.end() == "{") {
			downGrade.configListPushBack(downgradeConfigList(a));
		} else if (infoString.size() == 2) {
			std::list<std::string>::iterator tmpIter = infoString.begin()++;
			downGrade.configMapPushBack(*infoString.begin(), *tmpIter);
		}
	}
	return downGrade;
}

void	ConfigurationSingleton::fileParse(std::list<std::string> inputFile) {
	std::list<std::string>::iterator a = inputFile.begin(); // создал итератор на начало
	LocationInfo treeHead("root", "base"); // создал объект в котором будет храниться голова дерева
	while (a != inputFile.end()) { // парсинг файла до последней строчки
		treeHead.configListPushBack(downgradeConfigList(a)); // рекурсивная функция
	}
	_tree = &treeHead; // голову списка сохраняем, как голову дерева
}
