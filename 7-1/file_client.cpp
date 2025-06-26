#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(const char* message);
int main(int argc, char* argv[]){
    int sd;
    FILE* fp;
    char buf[BUF_SIZE];
    int read_cnt;

    sockaddr_in serv_adr;

    if(argc != 3){
        std::cout << "Usage : " << argv[0] << " <IP> <port>\n";
        exit(1);
    }

    fp = fopen("receive.bat", "wb");
    sd = socket(PF_INET, SOCK_STREAM, 0);

    serv_adr = {};
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(atoi(argv[2]));
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

    connect(sd, (sockaddr*) &serv_adr, sizeof(serv_adr));

    while((read_cnt = read(sd, buf, BUF_SIZE)) != 0){
        fwrite((void*) buf, 1, read_cnt, fp);
    }

    write(sd, "Thank you", 10);
    fclose(fp);
    close(sd);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}