#include <iostream>
#include <arpa/inet.h>
void error_handling(char* message);

int main(int argc, char* argv[]){
    char* addr = "12.34.56.1234";
    struct sockaddr_in addr_inet;

    if(!(inet_aton(addr, &addr_inet.sin_addr))){
        error_handling("Conversion error!");
    }else{
        printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);
    }
    return 0;
}

void error_handling(char* message){
    std::cerr << message << std::endl;
    exit(1);
}