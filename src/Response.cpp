//
// Created by Attack Cristina on 08.09.2022.
//

#include "Response.hpp"

Response::Response() : _httpVersion("HTTP/1.1") {
    initStatusCodes();
}

Response::Response(const Response &copy) { *this = copy; }

Response &Response::operator=(const Response &src) {
    if (this != &src) {
        _httpVersion = src._httpVersion;
        _status = src._status;
        _body = src._body;
    }
    return *this;
}

Response::~Response() {}

std::string getFileName(ClientSocket client, Request request) {
    std::string file = "";

    for (std::list<LocationInfo*>::const_iterator it = client.getServer()->getLocations().begin();
            it != client.getServer()->getLocations().end(); it++) {
        if (!request.getBody().count("Referer")) {
            if (!(*it)->getLocation().compare(request.getBody().find("Request-URI")->second)) {
                for (std::list<LocationInfo*>::const_iterator itDownGrade = (*it)->getDownGradeList().begin();
                        itDownGrade != (*it)->getDownGradeList().end(); itDownGrade++) {
                    if (!(*itDownGrade)->getType().compare("index")) {
                        file += (*it)->getLocation();
                    }
                }
            }
        }
    }
    return file;
}

bool Response::generateResponse(ClientSocket client, int clientSocket, Request request, int readCounter) {
    std::stringstream response;
    static std::ifstream file;
    static bool headerFlag;
    int size;

    if (!headerFlag) {
        std::cout << "\t\t\tFileName -> " << getFileName(client, request) << std::endl;

// file.open("resources/Screen Shot 2022-08-16 at 4.17.59 PM.png", std::ios::in | std::ios::binary | std::ios::ate);
//        file.open("./" + request.getBody().find("Request-URI")->second, std::ios::in | std::ios::binary | std::ios::ate);
        file.open("page.html", std::ios::in | std::ios::binary | std::ios::ate);
//		file.open("resources/sample.mp3", std::ios::in | std::ios::binary | std::ios::ate);
		if (file.fail()) {
            perror("Error : can't open input file");
            exit(1);
        }
        size = file.tellg();
        std::cout << "Content size : " << size << std::endl;
        response << "HTTP/1.1 200 OK\r\n"
                 << "Version: HTTP/1.1\r\n"
                 // << "Content-Type: video/mp4; charset=utf-8\r\n"
                 // << "Content-Type: image/png; charset=utf-8\r\n"
                 << "Content-Type: text/html; charset=utf-8\r\n"
                 //            << "Content-Type: audio/mpeg; charset=utf-8\r\n"
                 << "Content-Length: " << size << "\r\n\r\n";
        if (send(clientSocket, response.str().c_str(), response.str().length(), 0) == SOCKET_ERROR) {
            perror("Error : send message failure");
            exit(SOCKET_ERROR); // correct it
        }
        headerFlag = true;
    }

    file.seekg(readCounter);
    /* порционная отправка ответа */
    char buff[READ_BUFFER_SIZE];
    file.read(buff, READ_BUFFER_SIZE);
//	std::cout << "socket : " << clientSocket << " | send : " << send(clientSocket, buff, READ_BUFFER_SIZE, 0)  << std::endl;
    send(clientSocket, buff, READ_BUFFER_SIZE, 0);
    readCounter += READ_BUFFER_SIZE;
    // if (send(clientSocket, buff, READ_BUFFER_SIZE, 0) == SOCKET_ERROR) {
    //     perror("Error : send message failure");
    //     exit(SOCKET_ERROR); // correct it
    // }
    // readCounter += READ_BUFFER_SIZE;
//	std::cout << "counter pos : " << file.tellg() << std::endl;
    if (file.eof()) {
        std::cout << "I'M DONE" << std::endl;
        headerFlag = false;
        file.clear();
        file.close();
        readCounter = 0;
        return true;
    }
    return false;
}

void Response::initStatusCodes() {
    _statusCodes[100] = "Continue";
    _statusCodes[101] = "Switching Protocols";
    _statusCodes[102] = "Processing";
    _statusCodes[103] = "Checkpoint";
    _statusCodes[200] = "OK";
    _statusCodes[201] = "Created";
    _statusCodes[202] = "Accepted";
    _statusCodes[203] = "Non-Authoritative Information";
    _statusCodes[204] = "No Content";
    _statusCodes[205] = "Reset Content";
    _statusCodes[206] = "Partial Content";
    _statusCodes[207] = "Multi-Status";
    _statusCodes[300] = "Multiple Choices";
    _statusCodes[301] = "Moved Permanently";
    _statusCodes[302] = "Found";
    _statusCodes[303] = "See Other";
    _statusCodes[304] = "Not Modified";
    _statusCodes[305] = "Use Proxy";
    _statusCodes[306] = "Switch Proxy";
    _statusCodes[307] = "Temporary Redirect";
    _statusCodes[400] = "Bad Request";
    _statusCodes[401] = "Unauthorized";
    _statusCodes[402] = "Payment Required";
    _statusCodes[403] = "Forbidden";
    _statusCodes[404] = "Not Found";
    _statusCodes[405] = "Method Not Allowed";
    _statusCodes[406] = "Not Acceptable";
    _statusCodes[407] = "Proxy Authentication Required";
    _statusCodes[408] = "Request Timeout";
    _statusCodes[409] = "Conflict";
    _statusCodes[410] = "Gone";
    _statusCodes[411] = "Length Required";
    _statusCodes[412] = "Precondition Failed";
    _statusCodes[413] = "Request Entity Too Large";
    _statusCodes[414] = "Request-URI Too Long";
    _statusCodes[415] = "Unsupported Media Type";
    _statusCodes[416] = "Requested Range Not Satisfiable";
    _statusCodes[417] = "Expectation Failed";
    _statusCodes[418] = "I\'m a teapot";
    _statusCodes[422] = "Unprocessable Entity";
    _statusCodes[423] = "Locked";
    _statusCodes[424] = "Failed Dependency";
    _statusCodes[425] = "Unordered Collection";
    _statusCodes[426] = "Upgrade Required";
    _statusCodes[449] = "Retry With";
    _statusCodes[450] = "Blocked by Windows Parental Controls";
    _statusCodes[500] = "Internal Server Error";
    _statusCodes[501] = "Not Implemented";
    _statusCodes[502] = "Bad Gateway";
    _statusCodes[503] = "Service Unavailable";
    _statusCodes[504] = "Gateway Timeout";
    _statusCodes[505] = "HTTP Version Not Supported";
    _statusCodes[506] = "Variant Also Negotiates";
    _statusCodes[507] = "Insufficient Storage";
    _statusCodes[509] = "Bandwidth Limit Exceeded";
    _statusCodes[510] = "Not Extended";
}
