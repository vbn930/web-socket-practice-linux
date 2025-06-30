#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

void error_handling(const char* message);
int main(int argc, char* argv[]){
    int snd_buf = 1024*3, rcv_buf = 1024*3, state;
    int sock;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(snd_buf);

    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
    if(state){
        error_handling("setsockopt()");
    }

    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
    if(state){
        error_handling("setsockopt()");
    }

    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if(state){
        error_handling("getsockopt()");
    }

    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&rcv_buf, &len);
    if(state){
        error_handling("getsockopt()");
    }

    std::cout << "Input buffer size: " << snd_buf << "\n";
    std::cout << "Output buffer size: " << rcv_buf << "\n";

    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}