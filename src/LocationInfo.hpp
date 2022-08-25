#ifndef LOCATION_INFO_HPP
# define LOCATION_INFO_HPP

# include <list>
# include <map>
# include <iostream>
# include <string>
# include <sstream>
class LocationInfo {
	private :
		std::string _type;
		std::map<std::string, std::string> _config;
		
	public :
		LocationInfo();
		LocationInfo(const LocationInfo& copy);
		LocationInfo& operator = (const LocationInfo& op);
		void	configPushBack(std::string key, std::string value);
};

#endif