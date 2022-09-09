#ifndef CLIENT_SOCKET_HPP
# define CLIENT_SOCKET_HPP

# include "Request.hpp"
# include "Response.hpp"

class Response;
class Request;

class ClientSocket {
	private:
		/* variables */
		int 			_fd;
		Request			_inputRequest;
		Response		_outputResponse;

	public:
		/* constructors */
		ClientSocket(int fd);
		ClientSocket(const ClientSocket& copy)  { *this = copy; }
		ClientSocket& operator = (const ClientSocket& op);
		~ClientSocket() {}

		/* public functions */
        void setRequest(int clientSocket);

		/* getters */
		int	getFD(void) const 					{ return _fd; }
		const Request&	getRequest(void) const	{ return _inputRequest; }
		const Response&	getResponse(void) const	{ return _outputResponse; }

};

#endif