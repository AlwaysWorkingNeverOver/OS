#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "shared.hpp"

const int BUFFER_SIZE = 2;
int* buffer;
std::mutex buffer_mutex;

void consumer(){
    int item; 

    for(int i = 0; i < 2; i++){
        sem_wait(full);
        buffer_mutex.lock();
        item = buffer[in];
        std::cout << "Consumed: " << item << std::endl;
        in = (in + 1 ) % BUFFER_SIZE;
        buffer_mutex.unlock();
        sem_post(empty);
    }
}

int main(){
    int smh_fd = shm_open("/consumerproducer", O_CREAT | O_RDWR, | S_IWUSR);
    ftruncate(shm_fd, BUFFER_SIZE * sizeof(int));
    buffer = static_cast<int*>(mmap(nullptr, BUFFER_SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, smh_fd, 0));

    empty = sem_open("/empty", O_CREAT, S_IRUSR | S_IWUSR, BUFFER_SIZE);
    full = sem_open("/full", O_CREAT, S_IRUSR | S_IWUSR, 0);

    std::thread consumer_thread(consumer);

    consumer_thread.join();

    sem_close(empty);
    sem_close(full);
    sem_unlock("/empty");
    sem_unlock("/full");

    munmap(buffer, BUFFER_SIZE * sizeof(int));
    shm_unlink("/consumerproducer");
    return 0;
}