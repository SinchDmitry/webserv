#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

# include "Server.hpp"

class ClientSocket {
	private:
		/* variables */
		int 				_fd;
		Request				_inputRequest;
		Response			_outputResponce;

	public:
		/* constructors */
		ClientSocket(int fd);
		ClientSocket(const ClientSocket& copy)  { *this = copy; }
		ClientSocket& operator = (const ClientSocket& op);
		~ClientSocket() {}

		/* public functions */


		/* getters */
		int	getFD(void) const 					{ return _fd; }
		const Request	getRequest(void) const	{ return _inputRequest; }
		const Response	getResponse(void) const	{ return _outputResponce; }

};

#endif