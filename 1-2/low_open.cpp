#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void){
    int fd;
    char buf[] = "Hello world!\n";

    fd = open("data.txt", O_CREAT|O_WRONLY|O_TRUNC);
    if(fd == -1){
        std::cerr << "open() error\n";
    }

    std::cout << "file descriptor: " << fd << "\n";

    if(write(fd, buf, sizeof(buf)) == -1){
        std::cerr << "write() error\n";
    }
    close(fd);
    return 0;
}