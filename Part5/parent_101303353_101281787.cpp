#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstdio>
#include <string>

struct Shared { 
    int multiple; 
    int counter; 
};
static sembuf P = {0, -1, 0};
static sembuf V = {0,  1, 0};

int main() {
    std::cout.setf(std::ios::unitbuf);

    int shmid = shmget(IPC_PRIVATE, sizeof(Shared), IPC_CREAT | 0600);
    Shared* sh = (Shared*)shmat(shmid, nullptr, 0);

    sh->multiple = 3;
    sh->counter = 0;

    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    semctl(semid, 0, SETVAL, 1);

    pid_t pid = fork();
    if (pid < 0){
         return 1;
    }
    if (pid == 0) {
        std::string shmid_str = std::to_string(shmid);
        std::string semid_str = std::to_string(semid);
    
        char* args[] = {
            (char*)"child",                               
            const_cast<char*>(shmid_str.c_str()),               
            const_cast<char*>(semid_str.c_str()),                   
            nullptr                                                 
        };
    
        execv("./child", args);
        _exit(1);  
    }

    while (true) {
        semop(semid, &P, 1);
        int c = sh->counter;
        int m = sh->multiple;
        std::cout << "[parent] " << getpid() << " counter = " << c;
        if (c % m == 0) {
            std::cout << "  |  " << c << " is a multiple of " << m;
        }
        std::cout << "\n";
        sh->counter = c + 1;
        semop(semid, &V, 1);

        if (c > 500) {
            break;
        }
        usleep(200000);
    }

    shmdt(sh);
    shmctl(shmid, IPC_RMID, nullptr);
    semctl(semid, 0, IPC_RMID);
    return 0;
}
