#include <iostream>
#include <unistd.h>
#define BUF_SIZ 30

int main(int argc, char* argv[]){
    int fds1[2];
    int fds2[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZ];
    pid_t pid;

    pipe(fds1);
    pipe(fds2);
    pid = fork();
    if(pid = 0){
        write(fds1[1], str1, sizeof(str1));
        read(fds2[0], buf, BUF_SIZ);
        std::cout << "Child proc output: " << buf << "\n";
    }else{
        read(fds1[0], buf, BUF_SIZ);
        std::cout << "Parent proc output: " << buf << "\n";
        write(fds2[1], str2, sizeof(str2));
        sleep(3);
    }

    return 0;
}