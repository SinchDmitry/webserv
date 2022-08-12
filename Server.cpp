#include "Server.hpp"

Server::Server() : _numOfListenSocket(5), _port(12000) {} // 5 - random value

Server::~Server(){}

int Server::initListningSocket() {
    struct sockaddr_in addr;    // информация об IP адресе

    /* https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2 */ 
    addr.sin_family = AF_INET;                          // IPv4 протоколы Интернет
    // addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);    
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(_port);          
  
    /* https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2 */
    int listningSocket = socket(addr.sin_family, SOCK_STREAM, 0);
    if (listningSocket == SOCKET_ERROR) {
        std::cerr << "Error : cannot create a socket" << std::endl;
        return -1;
    }
    /* привязка сокета IP-адресу */
    if (bind(listningSocket, (const sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Error : cannot bind a socket" << std::endl;
        close(listningSocket);
        return -1;
    }
    /* Подготовим сокета к принятию входящих соединений от клиентов. */
    if (listen(listningSocket, SOMAXCONN) == SOCKET_ERROR) {  
        std::cerr << "Error : listen failure" << std::endl;
        close(listningSocket);
        return -1;
    }
    return listningSocket;
}

int Server::initPoll(int listningSocket) {
    /* https://www.opennet.ru/man.shtml?topic=poll&category=2&russian=0 */
    pollfd fds[80];
    fds[0].fd = listningSocket;
    fds[0].events = POLLIN;
    return 0;
}

void Server::run(int listningSocket) {
    while (true) {
        /* ожидает запрос на установку TCP-соединения от удаленного хоста. */
        int clientSocket = accept(listningSocket, NULL, NULL);
        if (clientSocket == SOCKET_ERROR) {
            std::cerr << "Error : TCP connection failure" << std::endl;
            exit(END_ERROR);
        }
        std::string buffer = parseHTTPHead(clientSocket);
        sendTestMessage(clientSocket, buffer);
        close(clientSocket);
    }
    close(listningSocket);
}

std::string Server::parseHTTPHead(int clientSocket) {
     char sym;
        std::string buffer = "";
        while (true) {
            int byteIn = recv(clientSocket, &sym, 1, 0);
            if (byteIn > EMPTY_BUFFER) {
                buffer += sym;
                if (buffer.length() > 4 && buffer.substr(buffer.length() - 4) == "\r\n\r\n") {
                    break;
                }
            } else if (byteIn == EMPTY_BUFFER) {
                break;
            } else if (byteIn == SOCKET_ERROR) {
                std::cerr << "Error : failure reading from TCP" << std::endl;
            } 
        }
        buffer[buffer.length()] = '\0';
        return buffer;
}

void Server::sendTestMessage(int clientSocket, std::string buf) {
    std::stringstream response; // сюда будет записываться ответ клиенту
    std::stringstream response_body;

    /* тело ответа (HTML) */
    response_body << "<title>Test C++ HTTP Server</title>\n"
        << "<h1>Test page</h1>\n"
        << "<p>This is body of the test page...</p>\n"
        << "<h2>Request headers</h2>\n"
        << "<pre>" << buf << "</pre>\n" 
        << "<em><small>Test C++ Http Server</small></em>\n";

    /* весь ответ вместе с заголовками */
    response << "HTTP/1.1 200 OK\r\n"
        << "Version: HTTP/1.1\r\n"
        << "Content-Type: text/html; charset=utf-8\r\n"
        << "Content-Length: " << response_body.str().length()
        << "\r\n\r\n" 
        << response_body.str();

    int byteOut = send(clientSocket, response.str().c_str(),
        response.str().length(), 0);
        if (byteOut == SOCKET_ERROR) {
            std::cerr << "Error : send message failure" << std::endl;
        }
}

void    Server::createTestListSockets() {
    for (int i = 0; i < _numOfListenSocket; ++i) {
        int tmpFd = initListningSocket();
        if (tmpFd != SOCKET_ERROR) {
            _fds[i].fd = tmpFd;
            _fds[i].events = POLLIN;
        } else {
            exit(SOCKET_ERROR);
        }
    }
    int nfds = 1;
    while (true) {
        // poll(_fds, nfds, 30000);
    }
}

    // std::cout << listningSocket << std::endl;
