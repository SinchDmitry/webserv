#ifndef CONFIGURATIONSINGLETON_HPP
# define CONFIGURATIONSINGLETON_HPP

#include "LocationInfo.hpp"
#include <fstream>

class ConfigurationSingleton {
	private :
		static ConfigurationSingleton* 	_instance;
		int 							_port;
		std::string 					_ipAddress;
		std::list<LocationInfo>			_locationData;

		ConfigurationSingleton(void);
		ConfigurationSingleton(const ConfigurationSingleton& copy);
		ConfigurationSingleton& operator = (const ConfigurationSingleton& op);

		std::list<std::string> 	split(const std::string &s, char delim);
		std::list<std::string>	fileInit();
		void						fileParse(std::list<std::string> inputFile);

	public :
		static ConfigurationSingleton* getInstance(void);
} ;

#endif