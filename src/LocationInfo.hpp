#ifndef LOCATION_INFO_HPP
# define LOCATION_INFO_HPP

# include <list>
# include <map>
# include <iostream>
# include <string>
# include <sstream>
class LocationInfo {
	private :
		std::string 						_type;
		std::string 						_location;
		std::map<std::string, std::string> 	_config;
		std::list<LocationInfo>				_downGrade;
		
	public :
		LocationInfo();
		LocationInfo(std::string name, std::string location);
		LocationInfo(const LocationInfo& copy);
		LocationInfo& operator = (const LocationInfo& op);

		void	configMapPushBack(std::string key, std::string value);
		void	configListPushBack(LocationInfo config);

		void	setType(std::string type) { _type = type; }
		void	setLocation(std::string location) { _location = location; }
};

#endif