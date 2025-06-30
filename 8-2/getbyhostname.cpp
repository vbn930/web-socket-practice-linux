#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

void error_handling(const char* message);
int main(int argc, char* argv[]){
    struct hostent* host;
    if(argc != 2){
        std::cout << "Usage: " << argv[0] << "<addr>\n";
        exit(1);
    }

    host = gethostbyname(argv[1]);
    if(!host){
        error_handling("hostbyname()");
    }

    std::cout << "Official name: " << host->h_name << "\n";

    for (int i = 0; host->h_aliases[i]; i++)
    {
        std::cout << "Aliases " << i+1 << ": " << host->h_aliases[i] << "\n";
    }

    std::cout << "Address type: " << ((host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6") << "\n";

    for (int i = 0; host->h_addr_list[i]; i++)
    {
        std::cout << "IP addr " << i+1 << ": " << inet_ntoa(*(in_addr*)host->h_addr_list[i]) << "\n";
    }

    return 0;
    
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}