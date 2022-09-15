//
// Created by Attack Cristina on 07.09.2022.
//

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <map>
# include <string>
# include <list>
# include <iostream>
# include <cstring>

# include <sys/types.h>
# include <sys/socket.h>

# define END_ERROR          1
# define EMPTY_BUFFER		0
# define SOCKET_ERROR       -1
# define MAX_SHORT          32767
# define READ_BUFFER_SIZE   10000

class Request {
private:
    std::map<std::string, std::string>  _body;
    std::string                         _method;
    std::string                         _message;
//    std::string                         _root;

    std::list<std::string>  split(const std::string& str, std::string myDelim);
    bool                    readToBuffer( int clientSocket, bool isHeader );

public:
    Request();
    Request(std::string method);
    Request(const Request& copy);
    Request& operator=(const Request& src);
    ~Request();

    const std::map<std::string, std::string>&   getBody() const  { return _body; };
    const std::string&                          getMethod() const { return _method; };
    const std::string&                          getMessage() const { return _message; };
//    const std::string&                          getRoot() const { return _root; };

    void setMethod( const std::string method ) { _method = method; };
//    void setRoot( const std::string root ) { _root = root; };

    void bodyMapPushBack(std::string key, std::string value);
    void parseRequest(int clientSocket);
};

#endif //REQUEST_HPP
