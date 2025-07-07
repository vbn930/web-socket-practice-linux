#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <vector>
#include <algorithm>

#define BUF_SIZE 1024
#define EPOLL_SIZE 50

void error_handling(const char* message);

int main(int argc, char* argv[]){
    int sock_cnt = 0;
    std::vector<int> socks;

    int serv_sock, clnt_sock;
    int str_len;

    sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    int epfd, event_cnt;

    epoll_event event;
    epoll_event* ep_events;

    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>" << "\n"; 
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handling("socket()");
    }

    serv_addr = sockaddr_in{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serv_sock, (sockaddr*) & serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind()");
    }

    if(listen(serv_sock, 5) == -1){
        error_handling("listen()");
    }

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = (epoll_event*) malloc(sizeof(epoll_event) * EPOLL_SIZE);

    event.data.fd = serv_sock;
    event.events = EPOLLIN;

    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while(true){
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        for(int i = 0; i < event_cnt; i++){
            if(ep_events[i].data.fd == serv_sock){
                clnt_addr_size = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (sockaddr*) &clnt_addr, &clnt_addr_size);
                if(clnt_sock == -1){
                    break;
                }

                event.data.fd = clnt_sock;
                event.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);

                //소켓 디스크립터를 배열에 저장
                socks.push_back(clnt_sock);
                sock_cnt = socks.size();

                std::cout << "Client " << clnt_sock << " joined in chatting\n";

            }else{
                char message[BUF_SIZE];
                clnt_sock = ep_events[i].data.fd;
                str_len = read(clnt_sock, message, BUF_SIZE);
                message[str_len] = 0;

                if(str_len == 0){
                    epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_sock, NULL);
                    close(clnt_sock);
                    socks.erase(std::find(socks.begin(), socks.end(), clnt_sock)); // 클라이언트 연결이 종료되면 배열에서 삭제
                    std::cout << "closed client: " << clnt_sock << "\n";
                }else{
                    std::cout << "Message from client " << clnt_sock << ": " << message << "\n";
                    for(int j = 0; j < sock_cnt; j++){
                        std::cout << "Send message to client " << socks[j] << ", " << strlen(message) <<  "\n";
                        write(socks[j], message, strlen(message)); // 수신된 채팅메세지를 모든 소켓에 전송
                    }
                }
            }
        }
    }

    close(epfd);
    close(serv_sock);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}