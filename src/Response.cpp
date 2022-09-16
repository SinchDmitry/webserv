#include "Response.hpp"
#include <unistd.h>

Response::Response() : _httpVersion("HTTP/1.1"), _autoindex(false) {
    initStatusCodes();
    initContentTypes();
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

std::string Response::replace(std::string src, std::string s1, std::string s2) {
    std::string dest = "";
    size_t index;

    if (s1 == s2) {
        return src;
    }

    index = src.find(s1);

    while (index != std::string::npos) {
        dest = src.substr(0, index) + s2 + src.substr(index + s1.length());
        src = dest;
        index = src.find(s1);
    }

    return dest;
}

std::string Response::UriDecode(const std::string & sSrc) {
    std::map<std::string, std::string> uriSymbs;
    uriSymbs["%20"] = " ";
    uriSymbs["%22"] = "\"";
    uriSymbs["%25"] = "%";
    uriSymbs["%2D"] = "-";
    uriSymbs["%2E"] = ".";
    uriSymbs["%3C"] = "<";
    uriSymbs["%3E"] = ">";
    uriSymbs["%5C"] = "\\";
    uriSymbs["%5E"] = "^";
    uriSymbs["%5F"] = "_";
    uriSymbs["%60"] = "`";
    uriSymbs["%7B"] = "{";
    uriSymbs["%7C"] = "|";
    uriSymbs["%7D"] = "}";
    uriSymbs["%7E"] = "~";

    std::size_t found = sSrc.find("%");
    if (found != std::string::npos) {
        std::string sResult = replace(sSrc, sSrc.substr(found, 3),
                                      uriSymbs.find(sSrc.substr(found, 3))->second);
    //    std::cout << "\t\tFilename --- " << sResult << std::endl;
        return sResult;
    } else {
//        std::cout << "\t\tFilename --- " << sSrc << std::endl;
        return sSrc;
    }
}

//std::string Response::getFileName(ClientSocket client, Request request) {
//    for (std::list<LocationInfo*>::const_iterator it = client.getServer()->getLocations().begin();
//            it != client.getServer()->getLocations().end(); it++) {
//        if (!request.getBody().count("Referer")) {
//            if (!(*it)->getLocation().compare(request.getBody().find("Request-URI")->second)) {
//                if ((*it)->getConfigList().count("index")) {
//                    return (*it)->getConfigList().find("index")->second;
//                } else {
//                    return "index.html";
//                }
//            }
//        } else {
//            std::string file = request.getBody().find("Request-URI")->second;
//            return UriDecode(file.substr(1, file.length()));
//        }
//    }
//
//    return "null";
//}

std::string Response::getFileName(ClientSocket client, Request request) {
    std::list<LocationInfo*> serverLocation = client.getServer()->getLocations();
    std::string root;
    std::string requestURI = request.getBody().find("Request-URI")->second;
    if (requestURI.rfind("/") == requestURI.length() - 1 // last "/" => directory
        || requestURI.substr(requestURI.rfind("/") + 1).find(".") == std::string::npos) { // в подстроке после последнего "/" нет точки => не файл
        for (std::list<LocationInfo*>::const_iterator it = serverLocation.begin(); it != serverLocation.end(); it++) {
            if(!(*it)->getLocation().compare(requestURI)
                || !(*it)->getLocation().compare(requestURI.substr(0, requestURI.length() - 1))) { // ищем в локациях сервера совпадающую с Request-URI
                if ((*it)->getConfigList().count("index")) {
//                    std::cout << "INDEX" << std::endl;
                    _autoindex = false; // если есть поле index в локации, то пусть к файлу = ./<root>/<index>
                //    std::cout << "CURRENT ROOT " << (*it)->getConfigList().find("root")->second << std::endl;
				
                    return UriDecode("./" + (*it)->getConfigList().find("root")->second + (*it)->getConfigList().find("index")->second);
                } else {
//                    std::cout << "NOT INDEX" << std::endl;
                    _autoindex = true;
                    return "tmp.html";
                }
            }
        }
    } else { // файл
//        std::cout << "HERE" << std::endl;
        std::string host = request.getBody().find("Host")->second;
		host = host.substr(0, host.length() - 1);
//        std::cout << "HOST -> " << host << std::endl;

        std::string referer = request.getBody().find("Referer")->second;
//	    std::cout << "REF -> " << referer << std::endl;

        int found = referer.find(host) + host.length();
        root = referer.substr(found, referer.length() - 1 - found);
//        std::cout << "ROOT -> " << root << std::endl;

        if (root.rfind("/") == root.length() - 1) { root = root.substr(0, root.length() - 1); }
		if (root.empty()) { root = "/"; }
        std::cout << "NEW ROOT " << root << std::endl;
//        std::cout << "LEN -> " << host.substr(0, host.length() - 1).length() << std::endl;
//        root = referer.substr(referer.find_last_of(host.substr(0, host.length() - 1)) + 1);
//        root[root.length()] = '\0';
//        std::cout << referer << std::endl
//                << host << std::endl
//                << root << std::endl;
    //    std::cout << "CURRENT ROOT " << root << " | LENGTH : " << root.length() << std::endl;
        for (std::list<LocationInfo*>::const_iterator it = serverLocation.begin(); it != serverLocation.end(); it++) {
            std::string tmpRoot = (*it)->getLocation();
//       		 std::cout << "TMP ROOT " << tmpRoot << " | LENGTH : " << tmpRoot.length() << std::endl;
//            std::cout << tmpRoot.length() << std::endl
//                << root.length() << std::endl
//                << (tmpRoot.compare(root)) << std::endl << std::endl;
            if(!tmpRoot.compare(root)) {
//               || !(*it)->getLocation().compare(requestURI.substr(0, requestURI.length() - 1))) { // ищем в локациях сервера совпадающую с Request-URI
                std::string rootFromMap = (*it)->getConfigList().find("root")->second.substr();
                std::cout << "FOUND LOCATION " << rootFromMap << std::endl;
                if (root.rfind("/") == root.length()) {
                    std::cout << "CURRENT ROOT " << rootFromMap << std::endl;
                    return UriDecode("./" + rootFromMap.substr(0, rootFromMap.length() - 1) + request.getBody().find("Request-URI")->second);
                } else {


					/* !!! 👀👀 HERE WRONG WAY TO DOC & LIST AND CORRECT FOR HOME, WTF  👀👀 !!! */


					// std::cout << "Debag root " << "./" << rootFromMap.substr(0, rootFromMap.length() - 1) << " " << request.getBody().find("Request-URI")->second << rootFromMap << std::endl;
                    // return UriDecode("./" + request.getBody().find("Request-URI")->second);
                    return UriDecode("./" + rootFromMap.substr(0, rootFromMap.length() - 1) + request.getBody().find("Request-URI")->second);


					/* !!! 👀👀 HERE WRONG WAY TO DOC & LIST AND CORRECT FOR HOME, WTF  👀👀 !!! */


				}
            }
        }

        _autoindex = false;
//        if (!root.substr(root.length() - 2, root.length() - 1).compare("/")) {
////            return UriDecode("." + root.substr(0, root.length() - 2) + request.getBody().find("Request-URI")->second);
//            return UriDecode("./" + root + request.getBody().find("Request-URI")->second);
//        } else {
//            return UriDecode("./" + root + request.getBody().find("Request-URI")->second);
//        }
    }

    return "null";
}

bool Response::generateResponse(ClientSocket client, int clientSocket, Request request, int& readCounter) {
    std::stringstream response;
    static std::ifstream file;
    std::string fileName;
    static bool headerFlag;
	static int length;

    fileName = getFileName(client, request);
    file.open(fileName, std::ios::in | std::ios::binary | std::ios::ate);
    if (!headerFlag) {
        std::cout << "Response Header" << std::endl;
        std::time_t tt;
        time (&tt);
        char resDate[100];

        bodyMapPushBack("Server", client.getServer()->getName());
        if (strftime(resDate, sizeof(resDate), "%a, %d %b %Y %H:%M:%S GMT", std::localtime(&tt))) {
            bodyMapPushBack("Date", resDate);
        }
        std::cout << "\t\tFileName -> " << fileName << std::endl;

        struct stat attrib;
        stat(fileName.c_str(), &attrib);
        if (strftime(resDate, sizeof(resDate), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&(attrib.st_mtime)))) {
            bodyMapPushBack("Last-Modified", resDate);
        }

		if (file.fail()) {
			std::cout << fileName << std::endl;
            perror("Error : can't open input file");
            _status = std::make_pair(404, _statusCodes.find(404)->second);
//            exit(1);
        } else {
            _status = std::make_pair(200, _statusCodes.find(200)->second);
        }
		length = file.tellg();
        bodyMapPushBack("Content-Length", std::to_string(file.tellg()));
        bodyMapPushBack("Version", _httpVersion);
        bodyMapPushBack("Connection", "Closed");
        bodyMapPushBack("Content-Type", _contentTypes.find(fileName.substr(fileName.rfind(".") + 1))->second);
        response << _httpVersion << " " << _status.first << " " << _status.second << "\r\n";
        for (std::map<std::string, std::string>::const_iterator it = _body.begin(); it != _body.end(); it++) {
            response << it->first << ": " << it->second << "\r\n";
        }
        response << "\r\n";
        if (send(clientSocket, response.str().c_str(), response.str().length(), MSG_NOSIGNAL) == SOCKET_ERROR) {
            perror("Error : send message failure");
            exit(SOCKET_ERROR); // correct it
        }
        headerFlag = true;
    }

    file.seekg(readCounter);
    /* порционная отправка ответа */
	std::string buff(READ_BUFFER_SIZE, '0');
    file.read(&buff[0], READ_BUFFER_SIZE);
	//	std::cout << "socket : " << clientSocket << " | send : " << send(clientSocket, buff, READ_BUFFER_SIZE, 0)  << std::endl;
    if (send(clientSocket, (char *)buff.c_str(), READ_BUFFER_SIZE, MSG_NOSIGNAL) == SOCKET_ERROR) {
        // std::cout << "Error " << fileName << std::endl;
        perror("Error : send message failure");
		return false;
        //  exit(SOCKET_ERROR); // correct it
    }
    readCounter += READ_BUFFER_SIZE;
	//	std::cout << "counter pos : " << file.tellg() << std::endl;
    if (file.eof()) {
        std::cout << "I'M DONE" << std::endl;
        headerFlag = false;
        file.clear();
        file.close();
        readCounter = 0;
        return true;
    }
    file.close();
    return false;
}

void Response::bodyMapPushBack(std::string key, std::string value) {
    _body.insert(std::pair<std::string, std::string>(key, value));
}

void Response::initContentTypes() {
    _contentTypes["gif"] = "image/gif";
    _contentTypes["jpg"] = "image/jpeg";
    _contentTypes["jpeg"] = "image/jpeg";
    _contentTypes["png"] = "image/png";
    _contentTypes["svg"] = "image/svg+xml";
    _contentTypes["webp"] = "image/webp";
    _contentTypes["ico"] = "image/vnd.microsoft.icon";

    _contentTypes["css"] = "text/css";
    _contentTypes["csv"] = "text/csv";
    _contentTypes["html"] = "text/html";
    _contentTypes["htm"] = "text/html";
    _contentTypes["xml"] = "text/xml";
    _contentTypes["htm"] = "text/html";
    _contentTypes["pdf"] = "application/pdf";
    _contentTypes["mp3"] = "audio/mpeg";
    _contentTypes["mp4"] = "video/mp4";
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
