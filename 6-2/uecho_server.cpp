#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(const char* message);

int main(int argc, char* argv[]){
    int serv_sock;
    int str_len;
    socklen_t clnt_adr_sz;

    sockaddr_in serv_adr, clnt_adr;
    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        exit(1);  
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1){
        error_handling("socket()");
    }

    serv_adr = {};
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (sockaddr*) &serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind()");
    }

    while(true){
        char message[BUF_SIZE];
        clnt_adr_sz = sizeof(clnt_adr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (sockaddr*) &clnt_adr, &clnt_adr_sz);

        std::cout << "Message from client: " << message << "\n";

        std::cout << "Input (Q or q to quit): ";
        std::cin >> message;
        if(!strcmp(message, "Q") || !strcmp(message, "q")){
            break;
        }

        sendto(serv_sock, message, strlen(message), 0, (sockaddr*) &clnt_adr, clnt_adr_sz);
    }

    close(serv_sock);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}