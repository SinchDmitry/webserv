#ifndef LISTEN_SOCKET_HPP
# define LISTEN_SOCKET_HPP

# include "Server.hpp"

class ListenSocket {
	private:
		/* variables */
		int 						_port;
		std::string					_ip;
		std::string					_name;
		std::list<LocationInfo*>	_locations;

		/* functions */
		std::list<std::string>  split(const std::string& str, std::string myDelim);

	public:
		/* constructors */
		ListenSocket(LocationInfo* server);
		~ListenSocket() {}

		/* getters */
		int	getPort(void) const 				{ return _port; }
		int getLocationSize(void) const 		{ return _locations.size(); }
		const std::string&	getIP(void) const	{ return _ip; }
		const std::string&	getName(void) const	{ return _name; }

};

// std::ostream& operator << (std::ostream& os, const ListenSocket& soc) {
// 	os	<< "Socket name : " << soc.getName() << std::endl 
// 		<< "IP address : " << soc.getIP() << std::endl 
// 		<< "Listen port : " << soc.getPort() << std::endl
// 		<< "Number of locations inside : " << soc.getLocationSize() << std::endl;
//     return os;
// }

#endif