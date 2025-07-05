#include <iostream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(const char* message);

int main(int argc, char* argv[]){
    int recv_sock;
    int str_len;
    char buf[BUF_SIZE];
    sockaddr_in addr;
    ip_mreq join_addr;

    if(argc != 3){
        std::cout << "Usage :" << argv[0] << " <GroupIP> <PORT>\n";
        exit(1);
    }

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    addr = sockaddr_in{};
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_family = AF_INET;
    
    if(bind(recv_sock, (sockaddr*) &addr, sizeof(addr)) == -1){
        error_handling("bind()");
    }

    join_addr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_addr.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*) &join_addr, sizeof(join_addr));

    while(true){
        str_len = recvfrom(recv_sock, buf, BUF_SIZE-1, 0, NULL, 0);
        if(str_len < 0){
            break;
        }
        buf[BUF_SIZE] = 0;
        fputs(buf, stdout);
    }
    close(recv_sock);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}