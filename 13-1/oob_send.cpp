#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 30
void error_handling(const char* message);

int main(int argc, char* argv[]){
    int sock;
    sockaddr_in recv_adr;
    char buf[BUF_SIZE];

    if(argc != 3){
        std::cout << "Usage: " << argv[0] << " <IP> <PORT>\n";
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    recv_adr = sockaddr_in{};
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (sockaddr*) &recv_adr, sizeof(recv_adr)) == -1){
        error_handling("connect()");
    }

    write(sock, "123", sizeof("123"));
    send(sock, "4", sizeof("4"), MSG_OOB);
    write(sock, "567", sizeof("567"));
    send(sock, "890", sizeof("890"), MSG_OOB);

    close(sock);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}