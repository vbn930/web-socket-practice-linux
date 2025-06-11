#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;

    if(argc != 3){
        std::cout << "Usage : " << argv[0] << " <IP> <port>\n";
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        std::cerr << "socket() error\n";
    }
    
    serv_addr = sockaddr_in{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1){
        std::cerr << "connect() error\n";
    }

    str_len = read(sock, message, sizeof(message)-1);
    if(str_len == -1){
        std::cerr << "read() error\n";
    }

    std::cout << "Message for server : " << message << "\n";
    close(sock);
    return 0;
}