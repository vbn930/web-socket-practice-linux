#include <iostream>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);
    
    std::cout << "Host ordered port: " << host_port << "\n";
    std::cout << "Network ordered port: " << net_port << "\n";
    std::cout << "Host ordered address: " << host_addr << "\n";
    std::cout << "Network ordered address: " << net_addr << "\n";
}