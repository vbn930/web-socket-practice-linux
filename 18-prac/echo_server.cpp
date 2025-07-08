#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define MAX_CLNT 256

void error_handling(const char* message);
void* handle_clnt(void* arg);

char* buf[BUF_SIZE];
pthread_mutex_t mutex;

int main(int argc, char* argv[]){
    int sock_serv, sock_clnt;
    sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;
    pthread_t t_id;

    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);

    sock_serv = socket(PF_INET, SOCK_STREAM, 0);
    
    serv_addr = sockaddr_in{};
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock_serv, (sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind()");
    }

    if(listen(sock_serv, 5) == -1){
        error_handling("listen()");
    }

    while(true){
        clnt_addr_sz = sizeof(clnt_addr);
        sock_clnt = accept(sock_serv, (sockaddr*) &clnt_addr, &clnt_addr_sz);

        pthread_create(&t_id, NULL, handle_clnt, (void*) sock_clnt);
        pthread_detach(t_id);
    }

    close(sock_serv);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}

void* handle_clnt(void* arg){
    int clnt_sock = *((int*) arg);
    int str_len = 0;

    while(1){
        pthread_mutex_lock(&mutex);
        str_len = read(clnt_sock, buf, BUF_SIZE);
        if(str_len == 0){
            break;
        }
        write(clnt_sock, buf, str_len);
        pthread_mutex_unlock(&mutex);
    }

    close(clnt_sock);
    return NULL;
}