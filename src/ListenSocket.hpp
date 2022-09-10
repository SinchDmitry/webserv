#ifndef LISTEN_SOCKET_HPP
# define LISTEN_SOCKET_HPP

# include <fstream>
# include <iostream>
# include "LocationInfo.hpp"

class ListenSocket {
	private:
		/* variables */
		int 						_port;
		std::string					_ip;
		std::string					_name;
		std::list<LocationInfo*>	_locations;
        int                         _fd;

		/* functions */
		std::list<std::string>  split(const std::string& str, std::string myDelim);

	public:
		/* constructors */
        ListenSocket() {}
		ListenSocket(LocationInfo* server);
		ListenSocket(const ListenSocket& copy)  { *this = copy; }
		ListenSocket& operator = (const ListenSocket& op);
		~ListenSocket() {}

		/* getters */
		int	getPort(void) const 				{ return _port; }
		int getLocationSize(void) const 		{ return _locations.size(); }
		const std::string&	getIP(void) const	{ return _ip; }
		const std::string&	getName(void) const	{ return _name; }
        const int getFd(void) const             { return _fd; }
        const std::list<LocationInfo*>& getLocations(void) const { return _locations; }

        void setFd(const int fd) { _fd = fd; }
};

static std::ostream& operator << (std::ostream& os, const ListenSocket& soc) {
	os	<< "================== start ==================" << std::endl
		<< "Socket name : " << soc.getName() << std::endl 
		<< "IP address : " << soc.getIP() << std::endl 
		<< "Listen port : " << soc.getPort() << std::endl 
		<< "Number of locations inside : " << soc.getLocationSize() << std::endl
		<< "=================== end ===================" << std::endl;
    return os;
}

#endif