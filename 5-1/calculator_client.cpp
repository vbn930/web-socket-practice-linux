#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(const char* message);

int main(int argc, char* argv[]){
    int sock;
    sockaddr_in sock_addr;
    char message[BUF_SIZE];

    if(argc != 3){
        std::cout << "Usage: " << argv[0] << " <IP> <PORT>\n";
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);

    sock_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (sockaddr*) &sock_addr, sizeof(sock_addr)) == -1){
        error_handling("connect()");
    }

    int num_cnt, num;
    char num_operator;

    std::cout << "Enter number count: ";
    std::cin >> num_cnt;

    message[0] = num_cnt + '0';
    int* nums = (int*) &message[1];

    for(int i = 0; i< num_cnt; i++){
        std::cout << "Enter number: ";
        std::cin >> num;
        nums[i] = num;
    }

    std::cout << "Enter operator: ";
    std::cin >> num_operator;

    message[num_cnt*sizeof(int)+1] = num_operator;

    write(sock, message, num_cnt*sizeof(int)+2);

    int result;
    read(sock, &result, BUF_SIZE);
    
    std::cout << "Result: " << result << std::endl;
    close(sock);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}