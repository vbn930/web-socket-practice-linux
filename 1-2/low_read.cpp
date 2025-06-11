#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100

int main(){
    int fd;
    char buf[BUF_SIZE];

    fd=open("data.txt", O_RDONLY);
    if(fd == -1){
        std::cerr << "open() error\n";
    }
    std::cout << "file descriptor: " << fd << "\n";

    if(read(fd, buf, sizeof(buf)) == -1){
        std::cerr << "read() error\n";
    }

    std::cout << "file data : "  << buf << "\n";
    close(fd);
    return 0;
}