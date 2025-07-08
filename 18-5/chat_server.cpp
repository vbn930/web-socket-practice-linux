#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <vector>
#include <algorithm>

#define BUF_SIZE 1024
#define MAX_CLNT 256
void error_handling(const char* message);
void* handle_clnt(void* arg);
void send_msg(char* msg, int len);

int clnt_cnt = 0;
std::vector<int> clnt_socks;
pthread_mutex_t mutex;

int main(int argc, char* argv[]){
    int sock_server, sock_clnt;
    char message[BUF_SIZE];
    int str_len = 0;
    sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    pthread_t t_id;

    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);

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

    while(true){
        clnt_addr_sz = sizeof(clnt_addr);
        sock_clnt = accept(sock_server, (sockaddr*) &clnt_addr, &clnt_addr_sz);

        pthread_mutex_lock(&mutex);
        clnt_socks.push_back(sock_clnt);
        clnt_cnt = clnt_socks.size();
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_clnt, (void*) &sock_clnt);
        pthread_detach(t_id);
        std::cout << "Connected client IP: " << inet_ntoa(clnt_addr.sin_addr) << "\n";
    }

    close(sock_server);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}

void* handle_clnt(void* arg){
    int clnt_sock = *((int*) arg);
    int str_len = 0;
    char msg[BUF_SIZE];
    while((str_len = read(clnt_sock, msg, BUF_SIZE) != 0)){
        send_msg(msg, str_len);
    } // 채팅이 종료되면 str_len이 0이되고, 이후에 소켓을 삭제

    pthread_mutex_lock(&mutex);
    clnt_socks.erase(std::find(clnt_socks.begin(), clnt_socks.end(), clnt_sock));
    clnt_cnt = clnt_socks.size();
    pthread_mutex_unlock(&mutex);

    close(clnt_sock);
    return NULL;
}

void send_msg(char* msg, int len){
    // 읽기 작업만 하는게 보장된 것이 아닌 다른 함수에서 배열에 대해 
    // 쓰기 작업이 발생하기 때문에 race condition 방지를 위해 lock을 해야한다.
    pthread_mutex_lock(&mutex); 
    for(auto clnt_sock : clnt_socks){
        write(clnt_sock, msg, len);
    }
    pthread_mutex_unlock(&mutex);
}