#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

int client_socket;

void *receive_msg(void *arg) {
    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int read_size = recv(client_socket, buffer, sizeof(buffer), 0);
        if (read_size <= 0) {
            printf("\nClient disconnected.\n");
            exit(0);
        }
        printf("\nClient: %s\n", buffer);
    }
}

void *send_msg(void *arg) {
    char message[1024];
    while (1) {
        fgets(message, sizeof(message), stdin);
        send(client_socket, message, strlen(message), 0);
    }
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    pthread_t send_thread, recv_thread;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 1);

    printf("Server waiting for connection...\n");

    client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    printf("Client connected!\n");

    pthread_create(&recv_thread, NULL, receive_msg, NULL);
    pthread_create(&send_thread, NULL, send_msg, NULL);

    pthread_join(recv_thread, NULL);
    pthread_join(send_thread, NULL);

    close(client_socket);
    close(server_fd);

    return 0;
}