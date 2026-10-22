
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080
#define SIZE 1024

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
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char message[SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Connection failed. Start the server first.\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to TCP chat server!\n");
    printf("Type exit to end the chat.\n");

    // Chat loop: send client message, then receive reply
    while (1) {
        printf("\nYou: ");

        if (fgets(message, SIZE, stdin) == NULL)
            break;

        if (!strchr(message, '\n')) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            printf("Message too long. Please try again.\n");
            continue;
        }

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

        int n = recv_line(client_socket, message, SIZE);

        if (n <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        printf("Server: %s", message);

        if (strcmp(message, "exit\n") == 0 ||
            strcmp(message, "exit") == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }

    // Close socket and clean up
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
