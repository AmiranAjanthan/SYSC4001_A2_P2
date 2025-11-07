#include <iostream>
#include <unistd.h>   

int main() {
    std::cout.setf(std::ios::unitbuf); 

    long x = 0;               
    unsigned long cycle = 0;  

    while (true) {
        std::cout << "[child ] " << getpid() << " cycle = " << cycle;
        if (x % 3 == 0) std::cout << "  |  " << x << " is a multiple of 3";
        std::cout << "\n";

        x--;        
        cycle++;    

        if (x < -500){
             break;
        }  
        usleep(200000);       
    }
    return 0;  
}
