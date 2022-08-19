#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sstream>
# include <unistd.h>
# include <string>
# include <sys/poll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>

# define END_ERROR          1
# define SOCKET_ERROR       -1
# define MAX_SHORT          32767
# define EMPTY_BUFFER       0
# define IP_ADDRESS         "127.0.0.1"
# define READ_BUFFER_SIZE   10000

class Server
{
    private:
		/* variables */
        pollfd      _fds[200];
        int         _port;
        int         _numOfListenSocket;
        int         _debagCounter;

		/* functions */
        bool        findInListenSockets(int fd);
		sockaddr_in setIdInfo();
		int  		addNewClientSocket(int &nfds, int i);
		void		closeClientSocket(int &nfds, int &i);
		int			waitForPoll(int nfds);

    public:
        Server();
        ~Server();

        int         initListningSocket();
        int         initPoll(int listningSocket);
        void        run();
        std::string readHTTPHead(int clientSocket);

        bool        sendTestMessage(int clientSocket, std::string buf, int &readCounter);
        void        createListSockets();
        void        closeListSockets();
};

#endif