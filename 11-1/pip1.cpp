#include <iostream>
#include <unistd.h>
#define BUF_SIZ 30

int main(int argc, char* argv[]){
    int fds[2];
    char str[] = "Who are you?";
    char buf[BUF_SIZ];
    pid_t pid;

    pipe(fds);
    pid = fork();
    if(pid = 0){
        write(fds[1], str, sizeof(str));
    }else{
        read(fds[0], buf, BUF_SIZ);
        puts(buf);
    }

    return 0;
}