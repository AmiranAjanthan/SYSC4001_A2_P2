#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstdlib>

struct Shared { 
    int multiple; 
    int counter; 
};
static sembuf P = {0, -1, 0};
static sembuf V = {0,  1, 0};

int main(int argc, char** argv) {
    std::cout.setf(std::ios::unitbuf);


    int shmid = std::atoi(argv[1]);
    int semid = std::atoi(argv[2]);

    Shared* sh = (Shared*)shmat(shmid, nullptr, 0);

    while (true) {
        semop(semid, &P, 1);
        int c = sh->counter;
        semop(semid, &V, 1);
        if (c > 100) {
            break;
        }
        usleep(200000);
    }

    while (true) {
        semop(semid, &P, 1);
        int c = sh->counter;
        int m = sh->multiple;
        semop(semid, &V, 1);

        std::cout << "[child ] " << getpid() << " sees counter = " << c;
        if (c % m == 0) {
            std::cout << "  |  " << c << " is a multiple of " << m;
        }
        std::cout << "\n";

        if (c > 500) {
            break;
        }
        usleep(200000);
    }

    shmdt(sh);
    return 0;
}
