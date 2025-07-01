#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    pid_t pid;
    int sock, state;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    pid = fork();

    if(pid = 0){
        std::cout << "Child sock id: " << sock << "\n";
        return 0;
    }else{
        std::cout << "Parent sock id: " << sock << "\n";
    }

    wait(&state);
    return 0;
}