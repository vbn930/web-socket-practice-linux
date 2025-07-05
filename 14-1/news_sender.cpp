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
    sockaddr_in mul_addr;
    int time_live = TTL;
    FILE* fp;
    char buf[BUF_SIZE];
    if(argc != 3){
        std::cout << "Usage :" << argv[0] << " <GroupIP> <PORT>\n"; 
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    mul_addr = sockaddr_in{};
    mul_addr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_addr.sin_port = htons(atoi(argv[2]));
    mul_addr.sin_family = AF_INET;

    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*) &time_live, sizeof(time_live));

    if((fp = fopen("news.txt", "r")) == NULL){
        error_handling("fopen()");
    }

    while(!feof(fp)){
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, (sockaddr*) &mul_addr, sizeof(mul_addr));
        sleep(2);
    }

    fclose(fp);
    close(send_sock);
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}