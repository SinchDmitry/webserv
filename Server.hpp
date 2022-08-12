#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sstream>
# include <unistd.h>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>

# define END_ERROR       1
# define SOCKET_ERROR    -1
# define MAX_SHORT       32767
# define EMPTY_BUFFER    0
# define IP_ADDRESS      "127.0.0.1"
# define PORT            81

class Server
{
    private:
       
    public:
        Server();
        ~Server();

        int     initListningSocket();
        void    run(int listningSocket);
};

#endif