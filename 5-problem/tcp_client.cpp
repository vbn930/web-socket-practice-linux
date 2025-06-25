#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    int str_len = 0;

    int idx = 0, read_len = 0;

    if(argc != 3){
        std::cout << "Usage : " << argv[0] << " <IP> <port>\n";
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        std::cerr << "socket() error\n";
        exit(1);
    }
    
    serv_addr = sockaddr_in{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1){
        std::cerr << "connect() error\n";
        exit(1);
    }

    char message[BUF_SIZE];
    int msg_size;

    for(int i = 0; i < 3; i++){
        str_len = read(sock, message, sizeof(message));
        int* received_size = (int*)&message[0];
        char* received_msg = (char*)&message[4];

        std::cout << "Message for server: " << received_msg << ", Size: " << received_size[0] << "\n";
        std::cout << "Read call count: " << str_len << "\n";

        char msg[BUF_SIZE];
        std::cout << "Input message: " << "\n";;
        std::cin >> msg;

        msg_size = strlen(msg);

        int* size_array = (int*) &message[0];
        size_array[0] = msg_size;
        char* msg_ptr = (char*) &message[4];
        strncpy(msg_ptr, msg, msg_size);
        std::cout << "Send message: " << msg_ptr << ", Size: " << size_array[0] << "\n";

        write(sock, message, msg_size+4);
    }
    close(sock);
    return 0;
}