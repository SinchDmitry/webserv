#include "ClientSocket.hpp"

ClientSocket::ClientSocket(int fd, ListenSocket* socket) : _fd(fd), _server(socket), _outputResponse(new Response()) {}

ClientSocket& ClientSocket::operator = (const ClientSocket& op) {
	if (this != &op) {
		_fd = op._fd;
        _server = op._server;
		_inputRequest = op._inputRequest;
		_outputResponse = op._outputResponse;
	}
	return *this;
}

void ClientSocket::setRequest(int clientSocket) {
    Request* request = new Request();
    request->parseRequest(clientSocket);
    _inputRequest = *request;
}

bool ClientSocket::setResponse(int clientSocket, int& readCounter) {
//    Response* response = new Response();
    bool result = _outputResponse->generateResponse(*this, clientSocket, _inputRequest, readCounter);
//    _outputResponse = response;
    return result;
}
