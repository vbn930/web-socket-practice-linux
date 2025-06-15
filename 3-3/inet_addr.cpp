#include <iostream>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    const char *addr1 = "1.2.3.4";
    const char *addr2 = "1.2.3.567";

    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr == INADDR_NONE){
        std::cout << "Error occured!\n";
    }else{
        printf("Network ordered integer addr: %#lx \n\n", conv_addr);
    }

    conv_addr = inet_addr(addr2);
    if(conv_addr == INADDR_NONE){
        std::cout << "Error occured!\n";
    }else{
        printf("Network ordered integer addr: %#lx \n\n", conv_addr);
    }
}