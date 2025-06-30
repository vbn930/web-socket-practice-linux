#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

void error_handling(const char* message);
int main(int argc, char* argv[]){
    int tcp_socket, udp_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_socket = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);

    std::cout << "SOCK_STREAM: " << tcp_socket << "\n";
    std::cout << "SOCK_DGRAM: " << udp_sock << "\n";

    state = getsockopt(tcp_socket, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state){
        error_handling("getsockopt()");
    }

    std::cout << "Socket type one: " << sock_type << "\n";

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state){
        error_handling("getsockopt()");
    }

    std::cout << "Socket type two: " << sock_type << "\n";

    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}