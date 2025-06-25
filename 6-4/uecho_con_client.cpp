#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(const char* message);

int main(int argc, char* argv[]){
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;

    sockaddr_in serv_adr, from_adr;
    if(argc != 3){
        std::cout << "Usage: " << argv[0] << " <IP> <PORT>\n";
        exit(1);  
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1){
        error_handling("socket()");
    }

    serv_adr = {};
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    connect(sock, (sockaddr*) &serv_adr, sizeof(serv_adr));

    while(true){
        std::cout << "Input (Q or q to quit): ";
        std::cin >> message;
        if(!strcmp(message, "Q") || !strcmp(message, "q")){
            break;
        }

        // sendto(sock, message, strlen(message), 0, (sockaddr*) &serv_adr, sizeof(serv_adr));
        // adr_sz = sizeof(from_adr);
        // str_len = recvfrom(sock, message, BUF_SIZE, 0, (sockaddr*) &from_adr, &adr_sz);

        write(sock, message, strlen(message));
        str_len = read(sock, message, sizeof(message) - 1);
        message[str_len] = 0;
        std::cout << "Message from server: " << message << "\n";
    }

    close(sock);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}