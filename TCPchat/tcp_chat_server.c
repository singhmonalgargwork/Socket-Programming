#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080
#define SIZE 1024

// Send the complete message
int send_all(SOCKET sock, const char *msg, int len) {
    int total = 0;

    while (total < len) {
        int n = send(sock, msg + total, len - total, 0);

        if (n <= 0)
            return 0;

        total += n;
    }

    return 1;
}

// Receive one line ending with newline
int recv_line(SOCKET sock, char *buffer, int size) {
    int i = 0;
    char ch;
    int n;

    while (i < size - 1) {
        n = recv(sock, &ch, 1, 0);

        if (n <= 0)
            return n;

        buffer[i++] = ch;

        if (ch == '\n')
            break;
    }

    buffer[i] = '\0';
    return i;
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr;
    char message[SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for a client
    if (listen(server_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("TCP Chat Server started on port %d.\n", PORT);
    printf("Waiting for a client...\n");

    // Accept one client
    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Client connected!\n");
    printf("You can type exit to end the chat.\n");

    // Chat loop: receive client message, then reply
    while (1) {
        int n = recv_line(client_socket, message, SIZE);

        if (n <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        printf("\nClient: %s", message);

        if (strcmp(message, "exit\n") == 0 ||
            strcmp(message, "exit") == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        printf("You: ");
        if (fgets(message, SIZE, stdin) == NULL)
            break;

        if (!send_all(client_socket, message,
                      (int)strlen(message))) {
            printf("Failed to send message.\n");
            break;
        }

        if (strcmp(message, "exit\n") == 0 ||
            strcmp(message, "exit") == 0) {
            printf("Chat ended.\n");
            break;
        }
    }

    // Close sockets and clean up
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    printf("Server closed.\n");
    return 0;
}
