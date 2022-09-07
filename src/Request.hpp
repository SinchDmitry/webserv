//
// Created by Attack Cristina on 07.09.2022.
//

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <list>

class Request {
private:
    std::map<std::string, std::string>  _body;
    std::string _method;

public:
    Request();
    Request(std::string method);
    Request(const Request& copy);
    Request& operator=(const Request& src);
    ~Request();

    const std::map<std::string, std::string>& getBody() const  { return _body; };
    const std::string& getMethod() const { return _method; };

    void bodyMapPushBack(std::string key, std::string value);
    void setMethod( const std::string method ) { _method = method; };
    void parseRequest( std::string buffer );
    std::list<std::string> split(const std::string& str, std::string myDelim);

};


#endif //REQUEST_HPP
