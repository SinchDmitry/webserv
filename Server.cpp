#include "Server.hpp"

Server::Server(){}

Server::~Server(){}

int Server::initListningSocket() {
    struct sockaddr_in addr;    // информация об IP адресе

    /* https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2 */ 
    addr.sin_family = AF_INET;                          // IPv4 протоколы Интернет
    // addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);    
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);          
  

    /* https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2 */
    int listningSocket = socket(addr.sin_family, SOCK_STREAM, 0);
    if (listningSocket == SOCKET_ERROR) {
        std::cout << "Error : cannot create a socket" << std::endl;
        return -1;
    }
    /* привязка сокета IP-адресу */
    if (bind(listningSocket, (const sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cout << "Error : cannot bind a socket" << std::endl;
        close(listningSocket);
        return -1;
    }
    /* Подготовим сокета к принятию входящих соединений от клиентов. */
    if (listen(listningSocket, SOMAXCONN) == SOCKET_ERROR) {  
        std::cout << "Error : listen failure" << std::endl;
        close(listningSocket);
        return -1;
    }
    return listningSocket;
}

void Server::run(int listningSocket) {
    while (true) {
        /* ожидает запрос на установку TCP-соединения от удаленного хоста. */
        int clientSocket = accept(listningSocket, NULL, NULL);
        if (clientSocket == SOCKET_ERROR) {
            std::cout << "error5" << std::endl;
            exit(END_ERROR);
        }
        char buf[MAX_SHORT];
        int byteIn = recv(clientSocket, buf, MAX_SHORT, 0);

        if (byteIn > EMPTY_BUFFER) {
            std::stringstream response; // сюда будет записываться ответ клиенту
            std::stringstream response_body;

            buf[byteIn] = '\0';

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

            std::cout << byteOut << std::endl;
        } else if (byteIn == EMPTY_BUFFER) {
            break;
        } else if (byteIn == SOCKET_ERROR) {
            std::cout << "Error : TCP connection failure" << std::endl;
        } 
        close(clientSocket);
    }
    close(listningSocket);
}
