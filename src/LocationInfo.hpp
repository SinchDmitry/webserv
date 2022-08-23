#ifndef LOCATION_INFO_HPP
# define LOCATION_INFO_HPP

# include <list>
# include <iostream>
# include <string>
# include <sstream>
class LocationInfo {
	private :
		std::string					_name;
		std::string 				_indexFile;
		std::string 				_rootDirectory;
		std::string 				_autoindexStatus;
		std::string					_redirect;
		std::list<std::string>		_metods;

		std::list<std::string> 		split(const std::string &s, char delim);

	public :
		LocationInfo();
		LocationInfo(const LocationInfo& copy);
		LocationInfo& operator = (const LocationInfo& op);

		void	setIndexFile(std::string indexFile);
		void	setRootDirectory(std::string rootDirectory);
		void	setAutoindexStatus(std::string autoindexStatus);
		void	addMethod(std::string method);
		
		const std::string&	getIndexFile(void) const;
		const std::string&	getRootDirectory(void) const;
		const std::string&	getAutoindexStatus(void) const;
		const std::list<std::string>&	getMetohods(void) const;
};

#endif