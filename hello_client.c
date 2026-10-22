#include <stdio.h>
#include <winsock2.h>

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];
    int bytes_received;

    // 1. Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }

    // 2. Create a TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // 3. Set the server address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // 4. Bind the socket to port 8080
    if (bind(server_socket, (struct sockaddr*)&server_addr,
             sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // 5. Listen for a client connection
    if (listen(server_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server is waiting on port 8080...\n");

    // 6. Accept one client
    client_socket = accept(server_socket, NULL, NULL);

    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed.\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Client connected.\n");
    printf("Received message: ");

    // 7. Receive and display data
    while ((bytes_received = recv(client_socket, buffer,
                                  sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytes_received, stdout);
        fflush(stdout);
    }

    if (bytes_received == SOCKET_ERROR) {
        printf("\nError receiving data.\n");
    } else {
        printf("\nClient closed the connection.\n");
    }

    // 8. Close sockets and clean up
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
