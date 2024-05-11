// クライアントサイドのコード

#define BUFSIZE 1000

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // close関数のためのヘッダー
#include <arpa/inet.h> // inet_addr関数のためのヘッダー

int main(int argc, char const *argv[])
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buf[BUFSIZE];
    int buf_len;

    // ソケットの生成
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("ERR: socket");
        exit(1);
    }

    // サーバーのアドレス・ポート番号を設定
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = PF_INET;                        // IPv4プロトコルを使用することを示す
    server_addr.sin_addr.s_addr = inet_addr("192.168.3.10"); // ここではLAN内のラズパイアドレスに指定
    server_addr.sin_port = htons(8000);                      // サーバーのポートを指定

    // ソケットをサーバーに接続
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("ERR: connect");
        close(sockfd);
        exit(1);
    }

    // ユーザーから文字列の読み取り
    buf_len = read(0, buf, BUFSIZE);
    if (buf_len < 0)
    {
        perror("ERR: read");
        close(sockfd);
        exit(1);
    }

    // メッセージの送信
    if (write(sockfd, buf, buf_len) < 0)
    {
        perror("ERR: write");
        close(sockfd);
        exit(1);
    }

    // メッセージの受信
    buf_len = read(sockfd, buf, BUFSIZE);
    if (buf_len < 0)
    {
        perror("ERR: read");
        close(sockfd);
        exit(1);
    }

    // 受信したメッセージを表示
    write(1, buf, buf_len);

    // ソケットを閉じる
    close(sockfd);
    return 0;
}
