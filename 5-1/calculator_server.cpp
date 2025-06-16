#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(const char* message);

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    char* operation_message = "Enter the operator: ";
    char message[BUF_SIZE];

    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        exit(1);  
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    serv_addr = sockaddr_in{};

    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind()");
    }

    if(listen(serv_sock, 5) == -1){
        error_handling("listen()");
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (sockaddr*) &clnt_addr, &clnt_addr_size);

    if(clnt_sock == -1){
        error_handling("accept()");
    }

    char num_operator;
    int str_len, recv_cnt, recv_len, num_cnt, num, result = 0;

    recv_cnt = 0;
    recv_len = 0;
    while(recv_cnt < recv_len){
        recv_cnt = read(clnt_sock, &message[recv_len], BUF_SIZE);
        if(recv_cnt == -1){
            error_handling("read()");
        }
        recv_len += recv_cnt;
    }
    
    num_cnt = message[0] - '0';
    num_operator = message[recv_len-1];
    int* nums = (int*)&message[1];
    result = nums[0];

    std::cout << "Number count: " << num_cnt << ", Numbers: " << result;

    for(int i = 1; i < num_cnt; i++){
        std::cout << " " << nums[i];
        if(num_operator == '+'){
            result += nums[i];
        }else if(num_operator == '-'){
            result -= nums[i];
        }else if(num_operator == '*'){
            result *= nums[i];
        }
    }

    std::cout << ", Operator: " << num_operator << ", Result: " << result << std::endl;

    write(clnt_sock, (char*)&result, sizeof(result));

    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}