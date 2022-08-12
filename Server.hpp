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

# define END_ERROR       1
# define SOCKET_ERROR    -1
# define MAX_SHORT       32767
# define EMPTY_BUFFER    0
# define IP_ADDRESS      "127.0.0.1"

class Server
{
    private:
        pollfd   _fds[200];
        int      _port;
        int      _numOfListenSocket;
    public:
        Server();
        ~Server();

        int         initListningSocket();
        int         initPoll(int listningSocket);
        void        run(int listningSocket);
        std::string parseHTTPHead(int clientSocket);

        void        sendTestMessage(int clientSocket, std::string buf);
        void        createTestListSockets();
};

#endif