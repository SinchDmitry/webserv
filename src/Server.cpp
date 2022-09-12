#include "Server.hpp"

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

/* private class functiones */
sockaddr_in Server::setIdInfo(ListenSocket serverInfo) {
	struct sockaddr_in addr;    // информация об IP адресе	
	/* https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2 */ 
	addr.sin_family = AF_INET;  // IPv4 протоколы Интернет
	addr.sin_addr.s_addr = inet_addr(serverInfo.getIP().c_str());    
	addr.sin_port = htons(serverInfo.getPort());
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
    // привязка серверного соккета к клиентскому
    std::list<ListenSocket*>::const_iterator it = _activeServers.begin();
    for (; it != _activeServers.end(); it++) {
        if ((*it)->getFd() == _fds[i].fd) {
            _activeClients.push_back(new ClientSocket(clientSocket, (*it)));
        }
    }
	nfds++;
	return 0;
}

void Server::closeClientSocket(int &nfds, int &i) {
	std::cout << "Connection closed : " << _fds[i].fd << std::endl;
	close(_fds[i].fd);
    for (std::list<ClientSocket *>::const_iterator it = _activeClients.begin(); it != _activeClients.end(); it++) {
        if ((*it)->getFD() == _fds[i].fd) {
            _activeClients.erase(it);
        }
    }
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
int Server::initListningSocket(ListenSocket serverInfo) {
	struct sockaddr_in addr = setIdInfo(serverInfo);
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

void Server::setRequestByFd(int fd) {
    for (std::list<ClientSocket*>::const_iterator it = _activeClients.begin(); it != _activeClients.end(); it++) {
        if ((*it)->getFD() == fd) {
            (*it)->setRequest(fd);
        }
    }
}

bool Server::setResponseByFd(int fd, int& readCounter) {
    for (std::list<ClientSocket*>::const_iterator it = _activeClients.begin(); it != _activeClients.end(); it++) {
        if ((*it)->getFD() == fd) {
            return (*it)->setResponse(fd, readCounter);
        }
    }
    return false;
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
            if (_fds[i].revents == 0) {
                continue;
            } else if (findInListenSockets(_fds[i].fd)) {
				if (addNewClientSocket(nfds, i)) {
					continue;
				}
            } else if (_fds[i].revents == POLLIN) {
                setRequestByFd(_fds[i].fd);
                _fds[i].events = POLLOUT;
                _fds[i].revents = 0;
            } else if (_fds[i].revents == POLLOUT) {
                if (setResponseByFd(_fds[i].fd, readCounter)) {
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

void    Server::createListSockets() {
	ConfigurationSingleton* infoFromConfig = infoFromConfig->getInstance();
	LocationInfo* root = (infoFromConfig->getTreeHead());
	/* поиск пространства с информацией о listen socket */
	while (!root->getDownGradeList().empty() && (*(root->getDownGradeList().begin()))->getType() != "server") {
		root = *(root->getDownGradeList().begin());
	}
	/* лист с информацией о listen socket ах, который надо обработать при инициализации сокетов */
	std::list<LocationInfo*>::const_iterator listOfServerIter = root->getDownGradeList().begin();
	// std::cout << (*listOfServerIter)->getType() << std::endl;
	_numOfListenSocket = root->getDownGradeList().size();
	/* инифиализация сокетов */
    for (int i = 0; i < _numOfListenSocket; ++i) {
		ListenSocket *newSocketFromConfig = new ListenSocket(*(listOfServerIter++));
        int tmpFd = initListningSocket(*newSocketFromConfig);
        std::cout << "Number  : " << i << " fd : " << tmpFd << std::endl;
        if (tmpFd != SOCKET_ERROR) {
            _fds[i].fd = tmpFd;
            _fds[i].events = POLLIN;
            newSocketFromConfig->setFd(tmpFd);
        }
        _activeServers.push_back(newSocketFromConfig);
    }
}

void    Server::closeListSockets() {
    for (int i = 0; i < _numOfListenSocket; ++i) {
        close (_fds[i].fd);
    }
}
