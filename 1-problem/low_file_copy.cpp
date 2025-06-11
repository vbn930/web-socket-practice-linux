#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100

int main(void){
    char file_name[] = "data.txt";

    int fd;
    char buf[BUF_SIZE];

    fd = open(file_name, O_RDONLY);

    ssize_t read_size = read(fd, buf, sizeof(buf));

    if(read_size == -1){
        std::cerr << "read() error\n";
        exit(1);
    }

    std::cout << "Read message : " << buf;

    close(fd);
    
    fd = open("data_copy.txt", O_CREAT|O_WRONLY|O_TRUNC);

    if(write(fd, buf, read_size) == -1){
        std::cerr << "write() error\n";
        exit(1);
    }

    close(fd);

    std::cout << "Data copy finished\n";
}