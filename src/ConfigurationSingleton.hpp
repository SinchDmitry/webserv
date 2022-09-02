#ifndef CONFIGURATIONSINGLETON_HPP
# define CONFIGURATIONSINGLETON_HPP

#include "LocationInfo.hpp"
#include <fstream>

class ConfigurationSingleton {
	private :
		LocationInfo*					_tree;

		ConfigurationSingleton(void);
		ConfigurationSingleton(const ConfigurationSingleton& copy);
		ConfigurationSingleton& operator = (const ConfigurationSingleton& op);

		std::list<std::string>  split(const std::string& str, std::string myDelim);
		std::list<std::string>	fileInit();
		void					fileParse(std::list<std::string> inputFile);
		void 					downgradeConfigList(LocationInfo& localHead, 
									std::list<std::string>::iterator& stringParserIterator);

	public :
		static ConfigurationSingleton* getInstance(void);
} ;

#endif