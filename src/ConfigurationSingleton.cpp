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
	buffer[size] = '\0';

	std::list<std::string> configInfo = split(buffer, '\n');

	/* print file */
	/*
		for (std::list<std::string>::iterator a = configInfo.begin(); a != configInfo.end(); a++) {
			std::cout << *a << std::endl;
		}
		std::cout << std::endl;
	*/
	return configInfo;
}

LocationInfo ConfigurationSingleton::downgradeConfigList(std::list<std::string>::iterator* stringParserIterator) {
	LocationInfo downGrade;
	std::list<std::string> infoString = split(**stringParserIterator, ' ');
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
		++(*stringParserIterator);
		infoString = split(**stringParserIterator, ' ');
		if (*infoString.begin() == "#") {
			continue;
		}
		std::list<std::string>::iterator tmpIter = --infoString.end();
		if (*tmpIter == "{" || *tmpIter == "}") {
			if (*tmpIter == "{") {
				// std::cout << "here open : "<< **stringParserIterator << 
					// " size : " << infoString.size() << std::endl;
				downGrade.configListPushBack(downgradeConfigList(stringParserIterator));
				continue;
			} 
			if (*tmpIter == "}") {
				// std::cout << "here close : "<< **stringParserIterator << 
					// " size : " << infoString.size() << std::endl;
				break;
			} 
		} else if (infoString.size() == 2) {
			std::list<std::string>::iterator tmpIter = infoString.begin()++;
			downGrade.configMapPushBack(*infoString.begin(), *tmpIter);
		}
	}
	return downGrade;
}

void	ConfigurationSingleton::fileParse(std::list<std::string> inputFile) {
	std::list<std::string>::iterator stringParserIterator = inputFile.begin(); // создал итератор на начало
	LocationInfo* treeHead = new LocationInfo("root", "base"); // создал объект в котором будет храниться голова дерева
	while (stringParserIterator != inputFile.end()) { // парсинг файла до последней строчки
		std::list<std::string> infoString = split(*stringParserIterator, ' ');
		if (*stringParserIterator != "" && *(--infoString.end()) == "{") {
			std::cout << "init open : "<< *stringParserIterator << 
				" size : " << infoString.size() << std::endl;
			treeHead->configListPushBack(downgradeConfigList(&stringParserIterator)); // рекурсивная функция
		}
		++stringParserIterator;
	}
	_tree = treeHead; // голову списка сохраняем, как голову дерева

	/* print file */
	while (true) {
		std::cout << "name : " << locationInf.getType();
		for (std::list<std::string>::iterator a = ; a != ; a++) {
			std::cout << *a << std::endl;
		}
		std::cout << std::endl;
		return configInfo;
	}
}

void recoursePrinter(LocationInfo locationInf) {
	
}