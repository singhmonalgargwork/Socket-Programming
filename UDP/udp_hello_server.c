// hello_server.c: Acts as the UDP client.

#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    const char message[] = "Hello, world!";

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    // Create a UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_socket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Configure the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // Send the datagram to the server
    int bytes_sent = sendto(
        client_socket,
        message,
        (int)strlen(message),
        0,
        (struct sockaddr *)&server_addr,
        sizeof(server_addr)
    );

    if (bytes_sent == SOCKET_ERROR) {
        printf("Message sending failed.\n");
    } else {
        printf("Message sent: %s\n", message);
    }

    // Close the socket and clean up
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
