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
    char *messages[] = {"Hi!", "I'm another UDP host!", "Nice to meet you!"};
    int str_len;
    socklen_t your_adr_sz;

    sockaddr_in your_adr;
    if(argc != 3){
        std::cout << "Usage: " << argv[0] << " <IP> <PORT>\n";
        exit(1);  
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1){
        error_handling("socket()");
    }

    your_adr = {};
    your_adr.sin_family = AF_INET;
    your_adr.sin_addr.s_addr = inet_addr(argv[1]);
    your_adr.sin_port = htons(atoi(argv[2]));

    for(int i = 0; i < 3; i++){
        sendto(sock, messages[i], strlen(messages[i]), 0, (sockaddr*) &your_adr, sizeof(your_adr));
    }

    close(sock);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}