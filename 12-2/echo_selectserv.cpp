#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUF_SIZE 1024
void error_handling(const char* message);

int main(int argc, char* argv[]){
    int sock_server, sock_clnt;
    char message[BUF_SIZE];
    int str_len = 0;
    sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    timeval timeout;
    fd_set reads, cpy_reads;

    int fd_max, str_len, fd_num;


    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        exit(1);
    }

    sock_server = socket(AF_INET, SOCK_STREAM, 0);
    
    serv_addr = sockaddr_in{};
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_family = AF_INET;

    if(bind(sock_server, (sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind()");
    }

    if(listen(sock_server, 5) == -1){
        error_handling("listen()");
    }

    FD_ZERO(&reads);
    FD_SET(sock_server, &reads);
    fd_max = sock_server;

    while(true){
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1){
            break;
        }

        if(fd_num == 0){
            continue;
        }

        for(int i = 0; i < fd_max + 1; i++){
            if(FD_ISSET(i, &cpy_reads)){
                if(i == sock_server){
                    clnt_addr_sz = sizeof(clnt_addr);
                    sock_clnt = accept(sock_server, (sockaddr*) &clnt_addr, &clnt_addr_sz);
                    FD_SET(sock_clnt, &reads);
                    if(fd_max < sock_clnt){
                        fd_max = sock_clnt;
                    }
                    std::cout << "connected client: " << sock_clnt << "\n";
                }else{
                    str_len = read(i , message, BUF_SIZE);
                    if(str_len == 0){
                        FD_CLR(i, &reads);
                        close(i);
                        std::cout << "closed client: "  << i << "\n";
                    }else{
                        write(i, message, str_len);
                    }
                }
            }
        }
    }
    close(sock_server);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}