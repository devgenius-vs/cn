#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8083

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[1024];

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    std::cout << "Connected to the Chat application." << std::endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (bytesRead <= 0) {
            std::cerr << "Error receiving data from client" << std::endl;
            continue;
        }

        std::cout << "Client: " << buffer << std::endl;

        std::string response;
        std::cout << "Server: ";
        std::getline(std::cin, response);
        sendto(serverSocket, response.c_str(), response.length(), 0, (struct sockaddr*)&clientAddr, clientAddrLen);
    }

    close(serverSocket);

    return 0;
}
