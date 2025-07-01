#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(const char* message);

int main(int argc, char* argv[]){
    int sock_server, sock_clnt;
    char message[BUF_SIZE];
    int str_len = 0;
    sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

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

    clnt_addr_sz = sizeof(clnt_addr);

    for(int i = 0; i < 5; i++){
        sock_clnt = accept(sock_server, (sockaddr*) &clnt_addr, &clnt_addr_sz);
        if(sock_clnt == -1){
            error_handling("accpet()");
        }else{
            std::cout << "Connected client: " << i+1 << "\n";
        }

        while((str_len = read(sock_clnt, message, BUF_SIZE)) != 0){
            write(sock_clnt, message, str_len);
        }

        close(sock_clnt);
    }

    close(sock_server);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}