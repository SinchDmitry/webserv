#include "ConfigurationSingleton.hpp"
#include <cstring>

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

std::list<std::string> ConfigurationSingleton::split(const std::string& str, std::string myDelim)
{
	std::list<std::string> dest;
	char* delim = (char *)myDelim.c_str();
    char* pTempStr = strdup( str.c_str() );      
    char* pWord = strtok(pTempStr, delim);       
    while(pWord != NULL)
    {
        dest.push_back(pWord);
        pWord = strtok(NULL, delim);        
    }
    
    free(pTempStr);
	return dest;
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

	std::list<std::string> configInfo = split(buffer, "\n");

	/* print file */
	// /*
		for (std::list<std::string>::iterator a = configInfo.begin(); a != configInfo.end(); a++) {
			std::cout << *a << std::endl;
		}
		std::cout << std::endl;
	// */
	return configInfo;
}

LocationInfo ConfigurationSingleton::downgradeConfigList(std::list<std::string>::iterator* stringParserIterator) {
	LocationInfo downGrade;
	std::list<std::string> infoString = split(**stringParserIterator, " \t\r\v\n\f");
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
		infoString = split(**stringParserIterator, " \t\r\v\n\f");
		if (*infoString.begin() == "#") {
			continue;
		}
		std::list<std::string>::iterator tmpIter = --infoString.end();
		if (*tmpIter == "{" || *tmpIter == "}") {
			if (*tmpIter == "{") {
				// std::cout << "here open : "<< **stringParserIterator << 
				// 	" size : " << infoString.size() << std::endl;
				downGrade.configListPushBack(downgradeConfigList(stringParserIterator));
				continue;
			} 
			if (*tmpIter == "}") {
				// std::cout << "here close : "<< **stringParserIterator << 
				// 	" size : " << infoString.size() << std::endl;
				break;
			} 
		} else if (infoString.size() == 2) {
			std::list<std::string>::iterator tmpIter = ++infoString.begin();
			downGrade.configMapPushBack(*infoString.begin(), *tmpIter);
		}
	}
	std::list<LocationInfo> location = downGrade.getDownGradeList();
	// std::cout << "size : " << locationInf->getType() << std::endl;
	if (!location.empty()) {
		for (std::list<LocationInfo>::iterator locationIter = location.begin(); 
			locationIter != location.end(); ++locationIter) {
				std::cout << "in recource : -==INCLUDE==-" << std::endl;
				// recoursePrinter(&*locationIter);
		}
	}
	return downGrade;
}

void recoursePrinter(LocationInfo* locationInf) {
	std::cout << "-==GENERAL==-" << std::endl;
	std::cout << "type : " << locationInf->getType() << std::endl;
	std::cout << "lctn : " << locationInf->getLocation() << std::endl;
	std::map<std::string, std::string> config = locationInf->getConfigList();
	if (!config.empty()) {
		std::cout << "-==DATA==-" << std::endl;
		for (std::map<std::string, std::string>::iterator configIter = config.begin(); 
			configIter != config.end(); ++configIter) {
			std::cout << "parameter : " << configIter->first << 
				" | value : " << configIter->second << std::endl;
		}
	}
	std::list<LocationInfo> location = locationInf->getDownGradeList();
	// std::cout << "size : " << locationInf->getType() << std::endl;
	if (!location.empty()) {
		for (std::list<LocationInfo>::iterator locationIter = location.begin(); 
			locationIter != location.end(); ++locationIter) {
				std::cout << "-==INCLUDE==-" << std::endl;
				recoursePrinter(&*locationIter);
		}
	}
	std::cout << "-==END of " << locationInf->getType() << " ==-" << std::endl;
}

void	ConfigurationSingleton::fileParse(std::list<std::string> inputFile) {
	std::list<std::string>::iterator stringParserIterator = inputFile.begin(); // создал итератор на начало
	LocationInfo* treeHead = new LocationInfo("root", "base"); // создал объект в котором будет храниться голова дерева
	while (stringParserIterator != inputFile.end()) { // парсинг файла до последней строчки
		std::list<std::string> infoString = split(*stringParserIterator, " \t\r\v\n\f");
		if (*stringParserIterator != "" && *(--infoString.end()) == "{") {
			// std::cout << "init open : "<< *stringParserIterator << 
				// " size : " << infoString.size() << std::endl;
			treeHead->configListPushBack(downgradeConfigList(&stringParserIterator)); // рекурсивная функция
		}
		++stringParserIterator;
	}
	_tree = treeHead; // голову списка сохраняем, как голову дерева

	std::cout << "==PRINTER==" << std::endl;
	recoursePrinter(_tree);
}
