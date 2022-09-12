//
// Created by Attack Cristina on 08.09.2022.
//

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <map>
# include <string>
# include <list>

# include <fstream>
# include <sstream>
# include <regex>

# include "Request.hpp"
# include "ClientSocket.hpp"

class Request;
class ClientSocket;

class Response {
private:
    std::string _httpVersion;
    std::pair<int, std::string> _status;
    std::map<std::string, std::string> _body;
    std::map<int, std::string> _statusCodes;
    void initStatusCodes();

    std::string replace(std::string src, std::string s1, std::string s2);
    std::string UriDecode(const std::string & sSrc);
    std::string getFileName(ClientSocket client, Request request);

public:
    Response();
    Response(const Response& copy);
    Response& operator=(const Response& src);
    ~Response();

    const std::string& getHttpVersion() const { return _httpVersion; };
    const std::pair<int, std::string>& getStatus() const { return _status; };
    const std::map<std::string, std::string>& getBody() const { return _body; };
    const std::map<int, std::string>& getStatusCodes() const { return _statusCodes; };

    bool generateResponse(ClientSocket client, int clientSocket, Request request, int readCounter);
};


#endif //RESPONSE_HPP
