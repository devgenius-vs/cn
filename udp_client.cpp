#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8083

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    while (true) {
        std::string message;
        std::cout << "Client: ";
        std::getline(std::cin, message);

        sendto(clientSocket, message.c_str(), message.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recvfrom(clientSocket, buffer, sizeof(buffer), 0, NULL, NULL);
        if (bytesRead <= 0) {
            std::cerr << "Error receiving data from server" << std::endl;
            continue;
        }

        std::cout << "Server: " << buffer << std::endl;
    }

    close(clientSocket);

    return 0;
}
