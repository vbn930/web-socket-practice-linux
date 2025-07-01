#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

void exit_handling(int signal);

int main(int argc, char* argv[]){
    char message[] = "Hello World!";
    signal(SIGINT, exit_handling);

    while(true){
        std::cout << message << "\n";
        sleep(1);
    }
}

void exit_handling(int signal){
    char* message;
    std::cout << "Enter Y to exit program: ";
    std::cin >> message;
    if(!strcmp("Y", message) || !strcmp("y", message)){
        exit(1);
    }else{
        return;
    }
}