#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100

int main(int argc, char *argv[]){
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    if(argc != 2){
        std::cout << "Usage : " << argv[0] << " <port>\n";
        exit(1);
    }

    //socket 함수는 소켓을 생성 후 해당 소켓에 대한 파일 디스럽터를 반환한다 (int 값).
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        std::cerr << "socket() error\n";
        exit(1);
    }
    
    serv_addr = sockaddr_in{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1){
        std::cerr << "bind() error\n";
        exit(1);
    }

    if(listen(serv_sock, 5)==-1){
        std::cerr << "listen() error\n";
        exit(1);
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1){
        std::cerr << "accept() error\n";
        exit(1);
    }

    //write 함수는 파일에 데이터를 출력 (소켓의 경우엔 전송) 하는 함수이다 -> 파일과 소켓에서 모두 사용한다.

    char message[BUF_SIZE];
    int msg_size, str_len;

    for(int i = 0; i < 3; i++){
        char msg[BUF_SIZE];
        std::cout << "Input message: " << "\n";;
        std::cin >> msg;

        msg_size = strlen(msg);

        int* size_array = (int*) &message[0];
        size_array[0] = msg_size;
        char* msg_ptr = (char*) &message[4];
        strncpy(msg_ptr, msg, msg_size);
        std::cout << "Send message: " << msg_ptr << ", Size: " << size_array[0] << "\n";

        write(clnt_sock, message, msg_size+4);

        str_len = read(clnt_sock, message, sizeof(message));
        int* received_size = (int*)&message[0];
        char* received_msg = (char*)&message[4];

        std::cout << "Message for server: " << received_msg << ", Size: " << received_size[0] << "\n";
        std::cout << "Read call count: " << str_len << "\n";
    }

    //close 함수는 파일, 소켓을 닫을때 공통적으로 사용한다 -> 파일과 소켓을 구분하지 않는 리눅스의 특성 때문이다.
    close(clnt_sock);
    close(serv_sock);
    return 0;
}