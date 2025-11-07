#include <iostream>
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/wait.h>   

int main() {
    std::cout.setf(std::ios::unitbuf);

    pid_t pid = fork();
    if (pid < 0){
         return 1;
    }

    if (pid == 0) {
        char *const argv[] = { (char*)"child", nullptr };
        execv("./child", argv);
        _exit(1);  
    }

    int status = 0;
    waitpid(pid, &status, 0);
    std::cout << "[parent] child " << pid << " finished (value < -500). Parent exit.\n";
    return 0;
}
