#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[100];
    int str_len = 0;

    int idx = 0, read_len = 0;

    if(argc != 3){
        std::cout << "Usage : " << argv[0] << " <IP> <port>\n";
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        std::cerr << "socket() error\n";
        exit(1);
    }
    
    serv_addr = sockaddr_in{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1){
        std::cerr << "connect() error\n";
        exit(1);
    }

    for(int i = 0; i < 3000; i++){
        std::cout << "Wait time: " << i << "\n";
    }
    
    str_len = read(sock, message, sizeof(message));

    std::cout << "Message for server: " << message << "\n";
    std::cout << "Read call count: " << str_len << "\n";
    close(sock);
    return 0;
}