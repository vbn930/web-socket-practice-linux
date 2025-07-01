#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
void error_handling(const char* message);
void read_childproc(int sig);

int main(int argc, char* argv[]){
    int sock_server, sock_clnt;
    char message[BUF_SIZE];
    int str_len = 0;
    sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    pid_t pid;
    struct sigaction act;
    int state;

    if(argc != 2){
        std::cout << "Usage: " << argv[0] << " <PORT>\n";
        exit(1);
    }

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    sock_server = socket(AF_INET, SOCK_STREAM, 0);
    
    serv_addr = sockaddr_in{};
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_family = AF_INET;

    if(bind(sock_server, (sockaddr*) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind()");
    }

    if(listen(sock_server, 5) == -1){
        error_handling("listen()");
    }

    while(true){
        clnt_addr_sz = sizeof(clnt_addr);

        sock_clnt = accept(sock_server, (sockaddr*) &clnt_addr, &clnt_addr_sz);

        if(sock_clnt == -1){
            continue;
        }else{
            puts("new clinet connected.....");
        }

        pid = fork();
        if(pid == -1){
            close(sock_clnt);
        }

        if(pid == 0){
            close(sock_server);
            while((str_len = read(sock_clnt, message, BUF_SIZE)) != 0){
                write(sock_clnt, message, str_len);
            }

            close(sock_clnt);
            puts("client disconnected.....");
            return 0;
        }
        else{
            close(sock_server);
        }
    }

    close(sock_server);
    return 0;
}

void error_handling(const char* message){
    std::cerr << message << " error!" << std::endl;
    exit(1);
}

void read_childproc(int sig){
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    std::cout << "Removed proc id: " << pid << "\n";
}