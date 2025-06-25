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
    int str_len, i;
    socklen_t adr_sz;
    sockaddr_in my_adr, your_adr;
    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        exit(1);  
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1){
        error_handling("socket()");
    }

    my_adr = {};
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (sockaddr*) &my_adr, sizeof(my_adr)) == -1){
        error_handling("bind()");
    }

    for(i = 0; i < 3; i++){
        sleep(5);
        adr_sz = sizeof(your_adr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (sockaddr*) &your_adr, &adr_sz);

        std::cout << "Message " << i << ": " << message << "\n";
    }

    close(sock);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}