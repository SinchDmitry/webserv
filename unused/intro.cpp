#include <iostream>
#include <sstream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define END_ERROR       1
#define SOCKET_ERROR    -1
#define MAX_SHORT       32767
#define EMPTY_BUFFER    0

int main() {
    struct addrinfo* addr = NULL;       // информация об IP адресе
    struct addrinfo hints;

    /* https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2 */ 
    hints.ai_family = AF_INET;          // IPv4 протоколы Интернет
    hints.ai_socktype = SOCK_STREAM;    /* Обеспечивает создание двусторонних надежных и
                                        последовательных потоков байтов , поддерживающих соединения.
                                        Может также поддерживаться механизм внепоточных данных. */
    hints.ai_protocol = IPPROTO_TCP;    // Используем протокол TCP
    hints.ai_flags = AI_PASSIVE;        /* сетевой адрес каждой структуры не будет указан. 
                                        Это используется серверными приложениями, 
                                        предназначенными для приема соединений клиентов, 
                                        имеющих любой сетевой адрес. */
    int result = getaddrinfo(NULL, "81", &hints, &addr);
    if (result) {
        std::cout << "error1" << std::endl;
        exit(END_ERROR);
    }

    /* https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2 */
    int listning = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (listning == SOCKET_ERROR) {
        std::cout << "error2" << std::endl;
        exit(END_ERROR);
    }

    result = bind(listning, addr->ai_addr, (int)addr->ai_addrlen); // привязка сокета IP-адресу
    if (result == SOCKET_ERROR) {
        std::cout << "error3" << std::endl;
        exit(END_ERROR);
    }

    result = listen(listning, SOMAXCONN);   // Подготовим сокета к принятию входящих соединений от клиентов.                                  
    if (result == SOCKET_ERROR) {
        std::cout << "error4" << std::endl;
        exit(END_ERROR);
    }

while (true) {

        int clientSocket = accept(listning, NULL, NULL); // ожидает запрос на установку TCP-соединения от удаленного хоста.
        if (clientSocket == SOCKET_ERROR) {
            std::cout << "error5" << std::endl;
            exit(END_ERROR);
        }
        char buf[MAX_SHORT];
        result = recv(clientSocket, buf, MAX_SHORT, 0);

        if (result > EMPTY_BUFFER) {
            std::stringstream response; // сюда будет записываться ответ клиенту
            std::stringstream response_body;

            buf[result] = '\0';

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

            result = send(clientSocket, response.str().c_str(),
                response.str().length(), 0);

            std::cout << result << std::endl;
        } else if (result == EMPTY_BUFFER) {
            break;
        } else if (result == SOCKET_ERROR) {
            std::cout << "error6" << std::endl;
        } 
    }
    close(listning);
    freeaddrinfo(addr);
}
