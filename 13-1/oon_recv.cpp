#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>

#define BUF_SIZE 30
void error_handling(const char* message);
void urg_handler(int signo);

int acpt_sock;
int recv_sock;

int main(int argc, char* argv[]){
    sockaddr_in recv_addr, serv_addr;
    int str_len, state;
    socklen_t serv_addr_sz;
    struct sigaction act;
    char buf[BUF_SIZE];
    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        exit(1); 
    }

    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    recv_addr = sockaddr_in{};
    recv_addr.sin_port = htons(atoi(argv[1]));
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_family = AF_INET;

    if(bind(acpt_sock, (sockaddr*) &recv_addr, sizeof(recv_addr)) == -1){
        error_handling("bind()");
    }

    listen(acpt_sock, 5);

    serv_addr_sz = sizeof(serv_addr);
    recv_sock = accept(acpt_sock, (sockaddr*) &serv_addr, &serv_addr_sz);

    fcntl(recv_sock, F_SETOWN, getpid());
    state = sigaction(SIGURG, &act, 0);

    while((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0){
        if(str_len == -1){
            continue;
        }

        buf[str_len] = 0;
        puts(buf);
    }

    close(recv_sock);
    close(acpt_sock);
    return 0;
}

void urg_handler(int signo){
    std::cout << "event" << "\n";
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = 0;
    std::cout << "Urgent message: " << buf << "\n";
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}