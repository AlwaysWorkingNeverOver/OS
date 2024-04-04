#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <fcnt.h>
#include <sys/mman.h>
#include "shared.h"
#include <ctime>
#include <cstdlib>

const int BUFFER_SIZE = 2;
int* buffer;
int in = 0; 

sem_t* empty;
sem_t* full;
std::mutex buffer_mutex;

void producer(){
    char item;
    int num;
    for(int i = 0; i < 2; i++){

        srand(time(nullptr));
        num = rand() % 26 + 65; 
        item = char(num); 
        sem_wait(empty);
        buffer_mutex.lock();
        buffer[in] = item;
        std::cout << "Produced: " << item << std::endl;
        in = (in + 1) % BUFFER_SIZE;
        buffer_mutix.unlock();
        sem_post(full);
    }
}

int main(){
    int shm_fd = shm_open("/consumerproducer", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shm_fd, BUFFER_SIZE * sizeof(itn));
    buffer = static_cast<int*>(mmap(nullptr, BUFFER_SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

    empty = sem_open("/empty", O_CREAT, S_IRUSR | S_IWUSR, BUFFER_SIZE);
    full = sem_open("/full", O_CREAT, S_IRUSR | S_IWUSR, 0);

    std::thread producer_thread(producer);
    producer_thread.join();

    sem_close(empty);
    sem_close(full);
    sem_unlink("/empty");
    sem_unlink("/full");

    munmap(buffer, BUFFER_SIZE * sizeof(int));
    shm_unlink("/consumerproducer"); 
    return 0; 
}