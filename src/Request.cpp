//
// Created by Attack Cristina on 07.09.2022.
//

#include "Request.hpp"

Request::Request() : _message("") {}

Request::Request(std::string method) : _method(method), _message("") {}

Request::Request(const Request &copy) { *this = copy; }

Request &Request::operator=(const Request &src) {
    if (this != &src) {
        _method = src._method;
        _message = src._message;
        _body = src._body;
    }
    return  *this;
}

Request::~Request() {}

void Request::bodyMapPushBack(std::string key, std::string value) {
    _body.insert(std::pair<std::string, std::string>(key, value));
}

bool Request::readToBuffer(int clientSocket, bool isHeader) {
    char sym;
    int byteIn = recv(clientSocket, &sym, 1, 0);
    if (byteIn > EMPTY_BUFFER) {
        _message += sym;
        if (isHeader && _message.length() > 4 && _message.substr(_message.length() - 4) == "\r\n\r\n") {
            return false;
        }
    } else if (byteIn == EMPTY_BUFFER) {
        return false;
    } else if (byteIn == SOCKET_ERROR) {
        perror("Error : failure reading from TCP");
    }
    return true;
}

void Request::parseRequest(int clientSocket) {
    // посимвольное считывание шапки запроса
    while (readToBuffer(clientSocket, true)) {}
    _message[_message.length()] = '\0';

    // сплит шапки по строкам
    std::list<std::string> rawData = split(_message, "\n");

    // сплит первой строки - метод, ссылка, версия протокола
    std::list<std::string> firstLine = split(*(rawData.begin()), " ");
    std::list<std::string>::const_iterator it = firstLine.begin();
    _method = *(it++);
    bodyMapPushBack("Request-URI", *(it++));
    bodyMapPushBack("HTTP-Version", *it);

    std::cout << "\tMethod -> " << _method << std::endl;
    std::cout << "\tRequest-URI-first-line -> " << _body.find("Request-URI")->second << std::endl;
    std::cout << "\tHTTP-Version -> " << _body.find("HTTP-Version")->second << std::endl;

    // создаем мапу из шапки
    it = ++rawData.begin();
    for(; it != rawData.end() && (*it).length() > 1; ++it) {
        bodyMapPushBack((*it).substr(0, (*it).find(":")), (*it).substr((*it).find(" ") + 1));
    }

    // если не ГЕТ, то посимвольно считываем сообщение после шапки
    _message = "";
    if (_method.compare("GET")) {
        for (int i = 0; i < stoi(_body.find("Content-Length")->second, nullptr, 10); i++) {
            readToBuffer(clientSocket, false);
        }
        _message[_message.length()] = '\0';
    }
    std::cout << "\tMessage -> " << _message << std::endl;
//    for (std::map<std::string, std::string>::iterator it = _body.begin();
//         it != _body.end(); ++it) {
//        std::cout << it->first << ": " << it->second << std::endl;
//    }
    std::cout << std::endl << "Host: " << _body.find("Host")->second << std::endl;
    if (_body.count("Referer")) {
        std::cout << std::endl << "Referer: " << _body.find("Referer")->second << std::endl;
    } else {
        std::cout << "Request-URI: " << _body.find("Request-URI")->second << std::endl << std::endl;
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
