#include "ClientSocket.hpp"

ClientSocket::ClientSocket(int fd) : _fd(fd) {}

ClientSocket& ClientSocket::operator = (const ClientSocket& op) {
	if (this != &op) {
		_fd = op._fd;
		_inputRequest = op._inputRequest;
		_outputResponse = op._outputResponse;
	}
	return *this;
}

void ClientSocket::setRequest(int clientSocket) {
    Request* request = new Request();
    request->parseRequest(clientSocket);
}

// void ClientSocket::setResponse() {
	
// }