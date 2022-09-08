#ifndef LOCATION_INFO_HPP
# define LOCATION_INFO_HPP

#include "Server.hpp"

class LocationInfo {
	private :
		/* variables */
		std::string 								_type;
		std::string 								_location;
		std::multimap<std::string, std::string> 	_config;
		std::list<LocationInfo*>					_downGrade;
		
	public :
		LocationInfo();
		LocationInfo(std::string name, std::string location);
		LocationInfo(const LocationInfo& copy);
		LocationInfo& operator = (const LocationInfo& op);
		~LocationInfo() {}

		void	configMapPushBack(std::string key, std::string value);
		void	configListPushBack(LocationInfo* config);

		void	setType(std::string type) 			{ _type = type; }
		void	setLocation(std::string location) 	{ _location = location; }

		const std::string& getType() const 										{ return _type; };
		const std::string& getLocation() const 									{ return _location; };
		const std::list<LocationInfo*>& getDownGradeList() const 				{ return _downGrade; };
		const std::multimap<std::string, std::string>& getConfigList() const 	{ return _config; };
};

#endif