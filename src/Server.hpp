#ifndef SERVER_HPP
# define SERVER_HPP

# include <unistd.h>
# include <sys/poll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <iostream>
# include <sstream>
# include <fstream>
# include <cstring>

# include "ConfigurationSingleton.hpp"
# include "ListenSocket.hpp"
# include "ClientSocket.hpp"

//# define END_ERROR          1
//# define EMPTY_BUFFER		0
//# define SOCKET_ERROR       -1
//# define MAX_SHORT          32767
//# define READ_BUFFER_SIZE   10000

class ListenSocket;
class ClientSocket;

class Server
{
    private:
		/* variables */
        pollfd      				_fds[200];
        int         				_numOfListenSocket;
		std::list<ClientSocket*> 	_activeClients;

		/* functions */
        bool            findInListenSockets(int fd);
		sockaddr_in     setIdInfo(ListenSocket serverInfo);
		int  		    addNewClientSocket(int &nfds, int i);
		void		    closeClientSocket(int &nfds, int &i);
		int			    waitForPoll(int nfds);

        void            setRequestByFd(int fd);
        bool            setResponseByFd(int fd, int readCounter);

    public:
        Server(){}
		~Server(){}

        int         initListningSocket(ListenSocket serverInfo);
        int         initPoll(int listningSocket);
        void        run();

        void        createListSockets();
        void        closeListSockets();
};

#endif