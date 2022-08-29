#ifndef CONFIGURATIONSINGLETON_HPP
# define CONFIGURATIONSINGLETON_HPP

#include "LocationInfo.hpp"
#include <fstream>

class ConfigurationSingleton {
	private :
		static ConfigurationSingleton* 	_instance;
		LocationInfo*					_tree;

		ConfigurationSingleton(void);
		ConfigurationSingleton(const ConfigurationSingleton& copy);
		ConfigurationSingleton& operator = (const ConfigurationSingleton& op);

		std::list<std::string> 	split(const std::string &s, char delim);
		std::list<std::string>	fileInit();
		void					fileParse(std::list<std::string> inputFile);
		LocationInfo 			downgradeConfigList(std::list<std::string>::iterator a);

	public :
		static ConfigurationSingleton* getInstance(void);
} ;

#endif