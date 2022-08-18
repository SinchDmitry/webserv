#include "Server.hpp"
#include <fstream>

/* debag funtions */
void    printFdsArray(pollfd *fds, int nfds) {
    std::cout << std::endl << "=========== open client sockets ===========" << std::endl;
    for (int i = 0; i < nfds; i++) {
        std::cout << i << " | fd : " << fds[i].fd << " | event : " 
            << fds[i].events << " | revent : " << fds[i].revents << std::endl;   
    }
    std::cout << "=================== end ===================" << std::endl << std::endl;
}

/* class constructors/destructors */
Server::Server() : _port(11000), _numOfListenSocket(5), _debagCounter(0) {} // 5 - random value

Server::~Server(){}

/* private class functiones */

sockaddr_in Server::setIdInfo() {
	struct sockaddr_in addr;    // информация об IP адресе	
	/* https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2 */ 
	addr.sin_family = AF_INET;  // IPv4 протоколы Интернет
	// addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);    
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(_port++);
	return addr;
}

bool Server::findInListenSockets(int fd) {
    for (int i = 0; i < _numOfListenSocket; ++i) {
        if (_fds[i].fd == fd) {
            return true;
        }
    }
    return false;
}

int Server::addNewClientSocket(int &nfds, int i) {
	int clientSocket = accept(_fds[i].fd, NULL, NULL);
	if (clientSocket == SOCKET_ERROR) {
		perror("Error : TCP connection failure");
		return SOCKET_ERROR;
	}
	std::cout << "Succsessful connection : " << _fds[i].fd << std::endl;
	_fds[nfds].fd = clientSocket;
	_fds[nfds].events = POLLIN;
	_fds[nfds].revents = 0;
	nfds++;
	return 0;
}

void Server::closeClientSocket(int &nfds, int &i) {
	std::cout << "Connection closed" << std::endl;
	close(_fds[i].fd);
	for (int j = i; j < nfds - 1; j++) {
		_fds[j] = _fds[j + 1];
	}
	--nfds;
	--i;
}

int Server::waitForPoll(int nfds) {
	int pollStatus = poll(_fds, nfds, -1);
	if (pollStatus == -1) {
		perror("Error : poll failure");
	} else if (!pollStatus) {
		perror("Error : poll timeout");
	}
	return pollStatus;
}

/* public clss functiones */

int Server::initListningSocket() {
	struct sockaddr_in addr = setIdInfo();    
	int listningSocket = socket(addr.sin_family, SOCK_STREAM, 0);
	if (listningSocket == SOCKET_ERROR) {
	    perror("Error : cannot create a socket");
	    return SOCKET_ERROR;
	}	
	/* фикс проблемы с "повисшим" bind */
	int enable = 1;
	if (setsockopt(listningSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
	    return SOCKET_ERROR;
	}	
	/* привязка сокета IP-адресу */
	if (bind(listningSocket, (const sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
	    perror("Error : cannot bind a socket");
	    close(listningSocket);
	    return SOCKET_ERROR;
	}
	/* Подготовим сокета к принятию входящих соединений от клиентов. */
	if (listen(listningSocket, SOMAXCONN) == SOCKET_ERROR) {  
	    perror("Error : listen failure");
	    close(listningSocket);
	    return SOCKET_ERROR;
	}
	return listningSocket;
}

void Server::run() {
    /* заполняем струтуру в которой будем хранить информацию о состоянии установленных соединений */
    createListSockets();
    int nfds = _numOfListenSocket;
    while (true) {
        /* ожидаем изменения состояния открытого сокета */
        if (waitForPoll(nfds) <= 0) {
			break;
		}
        /* ожидает запрос на установку TCP-соединения от удаленного хоста. */
        static int readCounter;
        for (int i = 0; i < nfds; ++i) {
            std::string buffer;
            if (_fds[i].revents == 0) {
                continue;
            } else if (findInListenSockets(_fds[i].fd)) {
				if (addNewClientSocket(nfds, i)) {
					continue;
				}
            } else if (_fds[i].revents == POLLIN) {
                buffer = readHTTPHead(_fds[i].fd);
                _fds[i].events = POLLOUT;
                _fds[i].revents = 0;
            } else if (_fds[i].revents == POLLOUT) {
                if (sendTestMessage(_fds[i].fd, buffer, readCounter)) {
                    _fds[i].events = POLLIN;
                }
                _fds[i].revents = 0;
            } else if (_fds[i].revents != POLLOUT && _fds[i].revents != POLLIN) {
                closeClientSocket(nfds, i);
            } else {
                perror("Error : wrong revent");
                break;
            }
        }
    }
    closeListSockets();
}

std::string Server::readHTTPHead(int clientSocket) {
    /* parsing head of HTTP request using one char buffer */
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

bool Server::sendTestMessage(int clientSocket, std::string buf, int& readCounter) {
    std::stringstream   response; // сюда будет записываться ответ клиенту
    // std::stringstream   response_body;
    static std::ifstream       file;
    static bool headerFlag;
    /* тело ответа (HTML) */
    // response_body << "<title>Test C++ HTTP Server</title>\n"
    //     << "<h1>Test page</h1>\n"
    //     << "<p>This is body of the test page...</p>\n"
    //     << "<h2>Request headers</h2>\n"
    //     << "<pre>" << buf << "</pre>\n" 
    //     << "<em><small>Test C++ Http Server</small></em>\n";

    
	int size;
    if (!headerFlag) {
        /* заголовок */
		file.open("/Users/aarchiba/Desktop/webserv/resources/videoplayback.mp4", std::ios::in | std::ios::binary | std::ios::ate);
		// file.open("/Users/aarchiba/Desktop/webserv/resources/Screen Shot 2022-08-16 at 4.17.59 PM.png", std::ios::in | std::ios::binary | std::ios::ate);
		// file.open("/Users/aarchiba/Desktop/webserv/page.html", std::ios::in | std::ios::binary | std::ios::ate);
		// file.open("/Users/aarchiba/Desktop/webserv/resources/sample.mp3", std::ios::in | std::ios::binary | std::ios::ate);
		if (file.fail()) {
			perror("Error : can't open input file");
			exit(1);
		}
		size = file.tellg();
		std::cout << "Content size : " << size << std::endl;
        response << "HTTP/1.1 200 OK\r\n"
            << "Version: HTTP/1.1\r\n"
            << "Content-Type: video/mp4; charset=utf-8\r\n"
            // << "Content-Type: image/png; charset=utf-8\r\n"
            // << "Content-Type: text/html; charset=utf-8\r\n"
            // << "Content-Type: audio/mpeg; charset=utf-8\r\n"
            << "Content-Length: " << size << "\r\n\r\n";
        if (send(clientSocket, response.str().c_str(), response.str().length(), 0) == SOCKET_ERROR) {
            perror("Error : send message failure");
            exit(SOCKET_ERROR); // correct it
        }
		file.seekg(0);
        headerFlag = true;
    }

    /* порционная отправка ответа */
    char buff[READ_BUFFER_SIZE];
    std::cout << file.read(buff, READ_BUFFER_SIZE) << std::endl;
	std::cout << "socket : " << clientSocket << " | send : " << send(clientSocket, buff, READ_BUFFER_SIZE, 0)  << std::endl;
    // if (send(clientSocket, buff, READ_BUFFER_SIZE, 0) == SOCKET_ERROR) {
    //     perror("Error : send message failure");
    //     exit(SOCKET_ERROR); // correct it
    // }
    // readCounter += READ_BUFFER_SIZE;
	std::cout << "counter pos : " << file.tellg() << std::endl;
    if (file.eof()) {
    // if (file.tellg() >= size) {
		std::cout << "I'M NOT ALIVE" << std::endl;
        headerFlag = false;
		file.clear();
        file.close();
        readCounter = 0;
        return true;
    }
    return false;
}

void    Server::createListSockets() {
    for (int i = 0; i < _numOfListenSocket; ++i) {
        int tmpFd = initListningSocket();
        std::cout << "Number  : " << i << " fd : " << tmpFd << std::endl;
        if (tmpFd != SOCKET_ERROR) {
            _fds[i].fd = tmpFd;
            _fds[i].events = POLLIN;
        }
    }
}

void    Server::closeListSockets() {
    for (int i = 0; i < _numOfListenSocket; ++i) {
        close (_fds[i].fd);
    }
}
