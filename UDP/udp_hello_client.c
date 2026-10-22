// hello_client.c: Acts as the UDP server

#include <stdio.h>
#include <winsock2.h>

#define PORT 8080
#define SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[SIZE + 1];
    int client_len = sizeof(client_addr);
    int bytes_received;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    // Create a UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to port 8080
    if (bind(server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("UDP server waiting on port %d...\n", PORT);

    // Receive one UDP datagram
    bytes_received = recvfrom(
        server_socket,
        buffer,
        SIZE,
        0,
        (struct sockaddr *)&client_addr,
        &client_len
    );

    if (bytes_received == SOCKET_ERROR) {
        printf("Receive failed.\n");
    } else {
        buffer[bytes_received] = '\0';
        printf("Received message: %s\n", buffer);
        printf("Datagram received successfully.\n");
    }

    // Close the socket and clean up
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
