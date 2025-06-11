#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]){
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello World";

    if(argc != 2){
        std::cout << "Usage : " << argv[0] << " <port>\n";
        exit(1);
    }

    //socket 함수는 소켓을 생성 후 해당 소켓에 대한 파일 디스럽터를 반환한다 (int 값).
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        std::cerr << "socket() error\n";
    }
    
    serv_addr = sockaddr_in{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1){
        std::cerr << "bind() error\n";
    }

    if(listen(serv_sock, 5)==-1){
        std::cerr << "listen() error\n";
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1){
        std::cerr << "accept() error\n";
    }

    //write 함수는 파일에 데이터를 출력 (소켓의 경우엔 전송) 하는 함수이다 -> 파일과 소켓에서 모두 사용한다.
    write(clnt_sock, message, sizeof(message));

    //close 함수는 파일, 소켓을 닫을때 공통적으로 사용한다 -> 파일과 소켓을 구분하지 않는 리눅스의 특성 때문이다.
    close(clnt_sock);
    close(serv_sock);
    return 0;
}