#ifndef CONFIGURATIONSINGLETON_HPP
# define CONFIGURATIONSINGLETON_HPP

#include "LocationInfo.hpp"
#include <fstream>
#include <algorithm>

class ConfigurationSingleton {
	private :
		LocationInfo*					_tree;

		ConfigurationSingleton(void);
		ConfigurationSingleton(const ConfigurationSingleton& copy);
		ConfigurationSingleton& operator = (const ConfigurationSingleton& op);

		std::list<std::string>  split(const std::string& str, std::string myDelim);
		std::list<std::string>	fileInit();
		void					fileParse(std::list<std::string> inputFile);
		LocationInfo 			downgradeConfigList(std::list<std::string>::iterator* a);

	public :
		static ConfigurationSingleton* getInstance(void);
} ;

#endif