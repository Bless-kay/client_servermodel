#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

int sock;

void *receive_msg(void *arg) {
    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int read_size = recv(sock, buffer, sizeof(buffer), 0);
        if (read_size <= 0) {
            printf("\nServer disconnected.\n");
            exit(0);
        }
        printf("\nServer: %s\n", buffer);
    }
}

void *send_msg(void *arg) {
    char message[1024];
    while (1) {
        fgets(message, sizeof(message), stdin);
        send(sock, message, strlen(message), 0);
    }
}

int main() {
    struct sockaddr_in serv_addr;

    pthread_t send_thread, recv_thread;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Connected to server!\n");

    pthread_create(&recv_thread, NULL, receive_msg, NULL);
    pthread_create(&send_thread, NULL, send_msg, NULL);

    pthread_join(recv_thread, NULL);
    pthread_join(send_thread, NULL);

    close(sock);

    return 0;
}
