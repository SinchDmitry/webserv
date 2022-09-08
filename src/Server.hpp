#ifndef SERVER_HPP
# define SERVER_HPP

# include <unistd.h>
# include <sys/poll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <list>
# include <map>
# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <cstring>
# include "ConfigurationSingleton.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "LocationInfo.hpp"
# include "ListenSocket.hpp"

# define END_ERROR          1
# define SOCKET_ERROR       -1
# define MAX_SHORT          32767
# define EMPTY_BUFFER       0
# define IP_ADDRESS         "127.0.0.1"
# define READ_BUFFER_SIZE   10000

class ListenSocket;

class Server
{
    private:
		/* variables */
        pollfd      _fds[200];
        int         _numOfListenSocket;

		/* functions */
        bool        findInListenSockets(int fd);
		sockaddr_in setIdInfo(ListenSocket serverInfo);
		int  		addNewClientSocket(int &nfds, int i);
		void		closeClientSocket(int &nfds, int &i);
		int			waitForPoll(int nfds);

    public:
        Server();
        ~Server();

        int         initListningSocket(ListenSocket serverInfo);
        int         initPoll(int listningSocket);
        void        run();
        std::string readHTTPHead(int clientSocket);

        bool        sendTestMessage(int clientSocket, std::string buf, int &readCounter);
        void        createListSockets();
        void        closeListSockets();
};

#endif