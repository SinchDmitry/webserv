//
// Created by Attack Cristina on 07.09.2022.
//

#include "Request.hpp"

Request::Request() {}

Request::Request(std::string method) : _method(method) {}

Request::Request(const Request &copy) { *this = copy; }

Request &Request::operator=(const Request &src) {
    if (this != &src) {
        _method = src._method;
        _body = src._body;
    }
    return  *this;
}

Request::~Request() {}

void Request::bodyMapPushBack(std::string key, std::string value) {
    _body.insert(std::pair<std::string, std::string>(key, value));
}

void Request::parseRequest(std::string buffer) {
    std::list<std::string> rawData = split(buffer, "\n");
    std::list<std::string>::const_iterator it = ++rawData.begin();
    if (!_method.compare("GET")) {
        for(; it != rawData.end() && (*it).length() > 1; ++it) {
            bodyMapPushBack((*it).substr(0, (*it).find(":")), (*it).substr((*it).find(" ") + 1));
        }
    } else {
        for(; it != rawData.end(); ++it) {
            if (it == rawData.end() - 2) {
                bodyMapPushBack((*it).substr(0, (*it).find(":")), (*it).substr((*it).find(" ") + 1));
            } else {
                bodyMapPushBack("Message-body", *it);
            }
        }

    }
}

std::list<std::string> Request::split(const std::string& str, std::string myDelim)
{
    std::list<std::string> dest;
    char* delim = (char *)myDelim.c_str();
    char* pTempStr = strdup( str.c_str() );
    char* pWord = strtok(pTempStr, delim);
    while(pWord != NULL)
    {
        dest.push_back(pWord);
        pWord = strtok(NULL, delim);
    }

    free(pTempStr);
    return dest;
}
