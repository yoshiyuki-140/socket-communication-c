// サーバー側
#define BUFSIZE 1000

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <unistd.h> // close関数のためのヘッダー
#include <arpa/inet.h> // htonl, htons関数のためのヘッダー

// 文字列内の小文字を大文字に変換する関数
char *StrToUpper(char *s)
{
    char *t;
    for (t = s; *t; t++)
    {
        *t = toupper(*t);
    }
    return s;
}

int main(int argc, char const *argv[])
{
    int sockfd, new_sockfd;
    socklen_t writer_len;
    struct sockaddr_in reader_addr, writer_addr;
    char buf[BUFSIZE];
    int buf_len;

    // ソケットの生成
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("ERR: socket");
        exit(1);
    }

    // 通信ポート・アドレスの設定
    bzero(&reader_addr, sizeof(reader_addr));
    reader_addr.sin_family = PF_INET;
    reader_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    reader_addr.sin_port = htons(8000);

    // ソケットにアドレスを割り当てる
    if (bind(sockfd, (struct sockaddr *)&reader_addr, sizeof(reader_addr)) < 0)
    {
        perror("ERR: bind");
        close(sockfd);
        exit(1);
    }

    // ソケットを接続待ちモードとする
    if (listen(sockfd, 3) < 0)
    {
        perror("ERR: listen");
        close(sockfd);
        exit(1);
    }

    // 接続要求を待つ
    writer_len = sizeof(writer_addr);
    if ((new_sockfd = accept(sockfd, (struct sockaddr *)&writer_addr, &writer_len)) < 0)
    {
        perror("ERR: accept");
        close(sockfd);
        exit(1);
    }

    // メッセージの受信と送信
    buf_len = read(new_sockfd, buf, BUFSIZE);
    if (buf_len < 0)
    {
        perror("ERR: read");
        close(new_sockfd);
        close(sockfd);
        exit(1);
    }
    write(1, buf, buf_len);
    write(new_sockfd, StrToUpper(buf), buf_len);

    // ソケットを閉じる
    close(new_sockfd);
    close(sockfd);

    return 0;
}
