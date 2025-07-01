#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
void error_handling(const char* message);
void read_routin(int sock, char* buf);
void write_routin(int sock, char* buf);

int main(int argc, char* argv[]){
    int sock;
    char message[BUF_SIZE];
    int str_len = 0;
    sockaddr_in sock_addr;

    pid_t pid;

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

    pid = fork();
    if(pid = 0){
        write_routin(sock, message);
    }else{
        read_routin(sock, message);
    }

    close(sock);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}

void read_routin(int sock, char* buf){
    while(true){
        int str_len = read(sock, buf, BUF_SIZE);
        if(str_len == 0){
            return;
        }
        buf[str_len] = 0;
        std::cout << "Message from server: " << buf << "\n";
    }
}

void write_routin(int sock, char* buf){
    while(true){
        std::cin >> buf;
        
        if(!strcmp(buf, "q") || !strcmp(buf, "Q")){
            shutdown(sock, SHUT_WR);
            return;
        }

        write(sock, buf, strlen(buf));
    }
}