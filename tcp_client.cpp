#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8082

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Unable to find the server" << std::endl;
        return 1;
    }

    std::cout << "Connected, Let's begin chatting!'" << std::endl;

    while (true) {
        std::string message;
        std::cout << "Client: ";
        std::getline(std::cin, message);

        send(clientSocket, message.c_str(), message.length(), 0);

        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cout << "Server disconnected" << std::endl;
            break;
        }

        std::cout << "Server: " << buffer << std::endl;
    }

    close(clientSocket);

    return 0;
}

