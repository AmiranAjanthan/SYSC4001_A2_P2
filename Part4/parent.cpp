#include <iostream>
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdio>       

struct Shared {
     int multiple; 
     int counter; 
    };

int main() {
    std::cout.setf(std::ios::unitbuf);

    int shmid = shmget(IPC_PRIVATE, sizeof(Shared), IPC_CREAT | 0600);
    if (shmid < 0){
         return 1;
    }

    Shared* sh = (Shared*)shmat(shmid, nullptr, 0);
    sh->multiple = 3;
    sh->counter  = 0;

    pid_t pid = fork();
    if (pid < 0){
         return 1;
    }

    if (pid == 0) {
        char *args[] = { (char*)"./bin/child", const_cast<char*>(std::to_string(shmid).c_str()), nullptr };
        execv("./child", argv);
        _exit(1);
    }

    while (sh->counter <= 500) {
        int c = sh->counter;
        int m = sh->multiple;
        std::cout << "[parent] " << getpid() << " counter = " << c;
        if (c % m == 0) {
            std::cout << "  |  " << c << " is a multiple of " << m;
        }
        std::cout << "\n";
        sh->counter = c + 1;

        usleep(200000);
    }

    shmdt(sh);
    shmctl(shmid, IPC_RMID, nullptr);
    return 0;
}
