#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50

void error_handling(const char* message);

int main(int argc, char* argv[]){
    int sock_server, sock_clnt;
    char message[BUF_SIZE];
    int str_len = 0;
    sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    epoll_event* ep_events;
    epoll_event event;
    int epfd, event_cnt;


    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        exit(1);
    }

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

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = (epoll_event*) malloc(sizeof(epoll_event) * EPOLL_SIZE);

    event.events = EPOLLIN;
    event.data.fd = sock_server;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sock_server, &event);

    while(true){
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if(event_cnt == -1){
            std::cerr << "epoll_wait() error!" << std::endl;
            break;
        }

        for(int i = 0; i < event_cnt; i++){
            if(ep_events[i].data.fd == sock_server){
                clnt_addr_sz = sizeof(clnt_addr);
                sock_clnt = accept(sock_server, (sockaddr*) &clnt_addr, &clnt_addr_sz);
                event.data.fd = sock_clnt;
                event.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, sock_clnt, &event);
                std::cout << "connected client: " << sock_clnt << "\n";
            }else{
                str_len = read(ep_events[i].data.fd, message, BUF_SIZE);
                if(str_len == 0){
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    std::cout << "closed clinet: " << ep_events[i].data.fd << "\n";
                }else{
                    write(ep_events[i].data.fd, message, str_len);
                }
            }
        }
    }
    
    close(sock_server);
    close(epfd);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}