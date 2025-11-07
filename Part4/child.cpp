#include <iostream>
#include <unistd.h>         
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>         

struct Shared { 
    int multiple; 
    int counter; 
};

int main(int argc, char** argv) {
    std::cout.setf(std::ios::unitbuf);
    int shmid = std::atoi(argv[1]);

    Shared* sh = (Shared*)shmat(shmid, nullptr, 0);

    while (sh->counter <= 100) {
        usleep(20000);
    }

    while (sh->counter <= 500) {
        int c = sh->counter;
        int m = sh->multiple;
        std::cout << "[child ] " << getpid() << " sees counter=" << c;
        
        if (c % m == 0) {
            std::cout << "  |  " << c << " is a multiple of " << m;
        }
        std::cout << "\n";
        usleep(200000);               
        }

    shmdt(sh);
    return 0;
}
