#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    const char *message = "Hello from client";

    // ソケットの作成
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // サーバーアドレスの設定
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // サーバーのIPアドレスを指定
    if (inet_pton(AF_INET, "192.168.3.7", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // サーバーに接続
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // メッセージの送信
    send(sock, message, strlen(message), 0);
    printf("Message sent\n");

    // メッセージの読み取り
    int valread = read(sock, buffer, BUFFER_SIZE);
    printf("Received message: %s\n", buffer);

    // ソケットを閉じる
    close(sock);

    return 0;
}

