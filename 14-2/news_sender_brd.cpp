#include <iostream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 30
void error_handling(const char* message);

int main(int argc, char* argv[]){
    int send_sock;
    sockaddr_in broad_addr;
    int so_broad = 1;
    FILE* fp;
    char buf[BUF_SIZE];
    if(argc != 3){
        std::cout << "Usage :" << argv[0] << " <BroadcastIP> <PORT>\n"; 
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    broad_addr = sockaddr_in{};
    broad_addr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_addr.sin_port = htons(atoi(argv[2]));
    broad_addr.sin_family = AF_INET;

    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*) &so_broad, sizeof(so_broad));

    if((fp = fopen("news.txt", "r")) == NULL){
        error_handling("fopen()");
    }

    while(!feof(fp)){
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (sockaddr*) &broad_addr, sizeof(broad_addr));
        sleep(2);
    }

    fclose(fp);
    close(send_sock);
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}