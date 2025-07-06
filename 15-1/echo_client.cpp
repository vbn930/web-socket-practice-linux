#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(const char* message);

int main(int argc, char* argv[]){
    int sock;
    char message[BUF_SIZE];
    int str_len = 0;
    sockaddr_in sock_addr;

    FILE* readfp;
    FILE* writefp;

    if(argc != 3){
        std::cout << "Usage: " << argv[0] << " <IP> <PORT>\n";
        exit(1);
    }
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        error_handling("sock()");
    }

    sock_addr = sockaddr_in{};
    sock_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (sockaddr*) &sock_addr, sizeof(sock_addr)) == -1){
        error_handling("connect()");
    }

    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w");

    while(true){
        std::cout << "Input message (Q to quit): ";
        std::cin >> message;
        
        if(!strcmp(message, "q") || !strcmp(message, "Q")){
            break;
        }

        fputs(message, writefp);
        fflush(writefp);
        fgets(message, BUF_SIZE, readfp);
        std::cout << "Message from server: " << message << "\n";
    }
    
    fclose(readfp);
    fclose(writefp);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}