#include "Server.hpp"

Server::Server() : _port(12000), _numOfListenSocket(5) {} // 5 - random value

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
        perror("Error : cannot create a socket");
        return -1;
    }
    /* привязка сокета IP-адресу */
    if (bind(listningSocket, (const sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        perror("Error : cannot bind a socket");
        close(listningSocket);
        return -1;
    }
    /* Подготовим сокета к принятию входящих соединений от клиентов. */
    if (listen(listningSocket, SOMAXCONN) == SOCKET_ERROR) {  
        perror("Error : listen failure");
        close(listningSocket);
        return -1;
    }
    return listningSocket;
}

bool Server::findInListenSockets(int fd) {
    for (int i = 0; i < _numOfListenSocket; ++i) {
        if (_fds[i].fd = fd) {
            return true;
        }
    }
    return false;
}

void Server::run(int listningSocket) {
    /* заполняем струтуру в которой будем хранить информацию о состоянии установленных соединений */
    createListSockets();
    int nfds = _numOfListenSocket;

    while (true) {
        /* ожидаем изменения состояния открытого сокета */
        int pollStatus = poll(_fds, nfds, 30000);
        if (pollStatus == -1) {
            perror("Error : poll failure");
            break;
        } else if (pollStatus == 0) {
            perror("Error : poll timeout");
            break;
        }
        /* ожидает запрос на установку TCP-соединения от удаленного хоста. */
        for (int i = 0; i < nfds; ++i) {
            if (_fds[i].revents == NULL) {
                continue;
            } else if (findInListenSockets(_fds[i].fd)) {
                int clientSocket = accept(_fds[i].fd, NULL, NULL);
                if (clientSocket == SOCKET_ERROR) {
                    perror("Error : TCP connection failure");
                    exit(END_ERROR);
                }
                std::cout << "Succsessful connection" << std::endl;
                _fds[nfds].fd = clientSocket;
                _fds[nfds].events = POLLIN;
                nfds++;
                std::string buffer = parseHTTPHead(clientSocket);
                sendTestMessage(clientSocket, buffer);
                close(clientSocket);
            }
        }
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
                perror("Error : failure reading from TCP");
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
            perror("Error : send message failure");
        }
}

void    Server::createListSockets() {
    for (int i = 0; i < _numOfListenSocket; ++i) {
        int tmpFd = initListningSocket();
        if (tmpFd != SOCKET_ERROR) {
            _fds[i].fd = tmpFd;
            _fds[i].events = POLLIN;
        } else {
            exit(SOCKET_ERROR);
        }
    }
}

    // std::cout << listningSocket << std::endl;
