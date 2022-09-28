#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <map>
# include <string>
# include <list>

# include <fstream>
# include <sstream>
# include <ios>
# include <ctime>
# include <sys/stat.h>

#include <dirent.h>
#include <errno.h> // - ? -
#include <sys/types.h>
#include <unistd.h>

# include "Request.hpp"
# include "ClientSocket.hpp"

class Request;
class ClientSocket;

class Response {
private:
    std::string                         _httpVersion;
    bool                                _autoindex;
    std::pair<int, std::string>         _status;
    std::map<std::string, std::string>  _body;
    std::map<int, std::string>          _statusCodes;
    std::map<std::string, std::string>  _contentTypes;

    void initStatusCodes();
    void initContentTypes();

    std::string replace(std::string src, std::string s1, std::string s2);
    std::string UriDecode(const std::string & sSrc);
    std::string getFileName(ClientSocket client, Request request);
    void        fillHeaders(ClientSocket client, std::string fileName, int contentLength);
    void        bodyMapPushBack(std::string key, std::string value);
    bool        lsHtml(std::string uri);
    bool        AIOnSend(ClientSocket client, int clientSocket, Request request, int& readCounter);
    bool        AIOffSend(ClientSocket client, int clientSocket, Request request, int& readCounter);

public:
    Response();
    Response(const Response& copy);
    Response& operator=(const Response& src);
    ~Response();

    const std::string&                          getHttpVersion() const { return _httpVersion; };
    const std::pair<int, std::string>&          getStatus() const { return _status; };
    const std::map<std::string, std::string>&   getBody() const { return _body; };
    const std::map<int, std::string>&           getStatusCodes() const { return _statusCodes; };

    bool generateResponse(ClientSocket client, int clientSocket, Request request, int& readCounter);
};


#endif //RESPONSE_HPP
