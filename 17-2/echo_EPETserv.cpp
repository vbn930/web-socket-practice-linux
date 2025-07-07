#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void setnonblockingmode(int fd);
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
    ep_events = (epoll_event*) malloc(sizeof(epoll_event) * EPOLL_SIZE); // epoll 인스턴스를 동적으로 할당 (생성될 소켓이 저장되는 배열)

    setnonblockingmode(sock_server);
    event.events = EPOLLIN;
    event.data.fd = sock_server;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sock_server, &event); // 서버 소켓을 epoll에 넣음

    while(true){
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1); // epoll에 있는 소켓에 이벤트가 발생할때 까지 대기
        if(event_cnt == -1){
            std::cerr << "epoll_wait() error!" << std::endl;
            break;
        }
        
        puts("return epoll_wait");

        for(int i = 0; i < event_cnt; i++){
            if(ep_events[i].data.fd == sock_server){ // 서버 소켓에 이벤트가 발생 = 클라이언트 소켓의 연결 요청
                clnt_addr_sz = sizeof(clnt_addr);
                sock_clnt = accept(sock_server, (sockaddr*) &clnt_addr, &clnt_addr_sz);
                setnonblockingmode(sock_clnt);
                event.data.fd = sock_clnt;
                event.events = EPOLLIN|EPOLLET; // Edge trigger 설정
                epoll_ctl(epfd, EPOLL_CTL_ADD, sock_clnt, &event); // 클라이언트 소켓을 epoll에 등록
                std::cout << "connected client: " << sock_clnt << "\n";
            }else{
                while(true){
                    str_len = read(ep_events[i].data.fd, message, BUF_SIZE);
                    if(str_len == 0){
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL); // epoll 인스턴스에서 종료된 클라이언트 소켓 삭제
                        close(ep_events[i].data.fd);
                        std::cout << "closed clinet: " << ep_events[i].data.fd << "\n";
                    }else if(str_len < 0){
                        if(errno == EAGAIN){
                            break;
                        }
                    }else{
                        write(ep_events[i].data.fd, message, str_len);
                    }
                }
            }
        }
    }
    
    close(sock_server);
    close(epfd);
    return 0;
}

void setnonblockingmode(int fd){
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}