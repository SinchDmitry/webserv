#ifndef CONFIGURATIONSINGLETON_HPP
# define CONFIGURATIONSINGLETON_HPP

#include "LocationInfo.hpp"
#include <fstream>

class ConfigurationSingleton {
	private :
		static ConfigurationSingleton* 		_instance;
		int 								_port;
		std::string 						_ipAddress;
		std::vector<LocationInfo>			_locationData;

		ConfigurationSingleton(void);
		ConfigurationSingleton(const ConfigurationSingleton& copy);
		ConfigurationSingleton& operator = (const ConfigurationSingleton& op);

		std::vector<std::string> 	split(const std::string &s, char delim);
		void						fileInit();
		void						fileParse();

	public :
		static ConfigurationSingleton* getInstance(void);
} ;

#endif