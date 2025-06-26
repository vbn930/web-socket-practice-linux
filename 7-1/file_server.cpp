#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(const char* message);
int main(int argc, char* argv[]){
    int serv_sd, clnt_sd;
    FILE* fp;
    char buf[BUF_SIZE];
    int read_cnt;

    sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        exit(1);  
    }

    fp = fopen("file_server.cpp", "rb");
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);

    serv_adr = {};
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(atoi(argv[1]));
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(serv_sd, (sockaddr*) &serv_adr, sizeof(serv_adr));
    listen(serv_sd, 5);

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sd = accept(serv_sd, (sockaddr*) &clnt_adr, &clnt_adr_sz);

    while(true){
        read_cnt = fread((void*) buf, 1, BUF_SIZE, fp);
        if(read_cnt < BUF_SIZE){
            write(clnt_sd, buf, read_cnt);
            break;
        }
        write(clnt_sd, buf, BUF_SIZE);
    }
    
    shutdown(clnt_sd, SHUT_WR);
    read(clnt_sd, buf, BUF_SIZE);
    std::cout << "Message from clinet: " << buf << "\n";

    close(clnt_sd);
    close(serv_sd);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}