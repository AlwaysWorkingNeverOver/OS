#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "shared.hpp"

const int BUFFER_SIZE = 2;
char* buffer;
int in; 
sem_t* empty;
sem_t* full;

void consumer() {
    char item;
    for (int i = 0; i < 2; i++) {
        sem_wait(full);
        item = buffer[i];
        std::cout << "Consumed: " << item << std::endl;
        in = (in + 1) % BUFFER_SIZE;
        sem_post(empty);
    }
}

int main() {
    int shm_fd = shm_open("/consumerproducer", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shm_fd, BUFFER_SIZE * sizeof(char));
    buffer = static_cast<char*>(mmap(nullptr, BUFFER_SIZE * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

    empty = sem_open("/empty", O_CREAT, S_IRUSR | S_IWUSR, BUFFER_SIZE);
    full = sem_open("/full", O_CREAT, S_IRUSR | S_IWUSR, 0);

    std::thread consumer_thread(consumer);

    consumer_thread.join();

    sem_close(empty);
    sem_close(full);
    sem_unlink("/empty");
    sem_unlink("/full");

    munmap(buffer, BUFFER_SIZE * sizeof(char));
    shm_unlink("/consumerproducer");

    return 0; 
}
