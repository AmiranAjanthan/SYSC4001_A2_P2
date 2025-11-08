#include <iostream>
#include <unistd.h>     
#include <sys/types.h> 
#include <cstdlib>      

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

    unsigned long cycle = 0;
    long x = 0;

    while (true) {
        std::cout << "[parent] " << getpid() << " cycle = " << cycle;
        if (x % 3 == 0) std::cout << "  |  " << x << " is a multiple of 3";
        std::cout << "\n";

        x++;         
        cycle++;     
        usleep(200000); 
    }
}
