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