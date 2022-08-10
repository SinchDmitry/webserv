#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define END_ERROR       1
#define SOCKET_ERROR    -1

int main() {
    struct addrinfo* addr = NULL; // информация об IP адресе
    struct addrinfo hints;
    /* https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=socket&category=2 */ 
    hints.ai_family = AF_INET; // IPv4 протоколы Интернет
    hints.ai_socktype = SOCK_STREAM; /* Обеспечивает создание двусторонних надежных и
    последовательных потоков байтов , поддерживающих соединения.
    Может также поддерживаться механизм внепоточных данных. */
    hints.ai_protocol = IPPROTO_TCP; // Используем протокол TCP
    hints.ai_flags = AI_PASSIVE; /* сетевой адрес каждой структуры не будет указан. 
    Это используется серверными приложениями, предназначенными для приема соединений клиентов, 
    имеющих любой сетевой адрес. */
    int result = getaddrinfo(NULL, "80", &hints, &addr);
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
    std::cout << result << std::endl;
    if (result == SOCKET_ERROR) {
        std::cout << "error3" << std::endl;
        exit(END_ERROR);
    }
}
