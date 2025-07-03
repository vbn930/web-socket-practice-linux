#include <iostream>
#include <unistd.h>
#define BUF_SIZ 30

int main(int argc, char* argv[]){
    int fds1[2];
    int fds2[2];
    char* strs[] = {"Message 1", "Message 2", "Message 3"};
    char buf[BUF_SIZ];
    pid_t pid;

    pipe(fds1);
    pipe(fds2);
    pid = fork();
    if(pid = 0){
        for(int i = 0; i < 3; i++){
            write(fds1[1], strs[i], sizeof(strs[i]));
            read(fds2[0], buf, BUF_SIZ);
            std::cout << "Child proc output: " << buf << "\n";
        }
    }else{
        for(int i = 0; i < 3; i++){
            write(fds1[1], strs[i], sizeof(strs[i]));
            read(fds2[0], buf, BUF_SIZ);
            std::cout << "Parent proc output: " << buf << "\n";
        }
        sleep(3);
    }

    return 0;
}