#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    char buffer[1024];
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding." << std::endl;
        return 1;
    }
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening." << std::endl;
        return 1;
    }
    std::cout << "Server listening on port 12345..." << std::endl;
    newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
    if (newSocket == -1) {
        std::cerr << "Error accepting connection." << std::endl;
        return 1;
    }
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(newSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            std::cerr << "Error receiving data." << std::endl;
            break;
        }
        if (bytesReceived == 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }
std::cout << "Received: " << buffer << std::endl;
if (strcmp(buffer, "exit") == 0) {
            std::cout << "Client requested an exit." << std::endl;
            break;
        }
        int bytesSent = send(newSocket, buffer, bytesReceived, 0);
        if (bytesSent == -1) {
            std::cerr << "Error sending data." << std::endl;
            break;
        }
    }
    close(newSocket);
    close(serverSocket);
    return 0;
}
