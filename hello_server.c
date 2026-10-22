#include <stdio.h>
#include <string.h>
#include <winsock2.h>

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    const char message[] = "Hello, world!";
    int total_sent = 0;
    int message_length = (int)strlen(message);

    // 1. Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    // 2. Create a TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // 3. Configure the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    // 4. Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr,
                sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed. Start the server first.\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to the server.\n");

    // 5. Send the entire message
    while (total_sent < message_length) {
        int sent = send(client_socket, message + total_sent,
                        message_length - total_sent, 0);

        if (sent == SOCKET_ERROR || sent == 0) {
            printf("Failed to send message.\n");
            closesocket(client_socket);
            WSACleanup();
            return 1;
        }

        total_sent += sent;
    }

    printf("Message sent: %s\n", message);

    // 6. Finish sending and close the connection
    shutdown(client_socket, SD_SEND);
    closesocket(client_socket);

    // 7. Release Winsock resources
    WSACleanup();

    return 0;
}
