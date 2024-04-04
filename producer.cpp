#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstdlib>

const int BUFFER_SIZE = 2;
char* buffer; // Changed to char* to store characters
int in = 0; 

sem_t* empty;
sem_t* full;

void producer() {
    char item;
    srand(time(nullptr));
    for (int i = 0; i < 2; i++) {
        char num = rand() % 26 + 65;
        item = char(num);
        sem_wait(empty);
        buffer[in] = item;
        std::cout << "Produced: " << item << std::endl;
        in = (in + 1) % BUFFER_SIZE;
        sem_post(full);
    }
}

int main() {
    int shm_fd = shm_open("/consumerproducer", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shm_fd, BUFFER_SIZE * sizeof(char)); // Changed to sizeof(char) for characters
    buffer = static_cast<char*>(mmap(nullptr, BUFFER_SIZE * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

    empty = sem_open("/empty", O_CREAT, S_IRUSR | S_IWUSR, BUFFER_SIZE);
    full = sem_open("/full", O_CREAT, S_IRUSR | S_IWUSR, 0);

    std::thread producer_thread(producer);

    producer_thread.join();

    sem_close(empty);
    sem_close(full);
    sem_unlink("/empty");
    sem_unlink("/full");

    munmap(buffer, BUFFER_SIZE * sizeof(char));
    shm_unlink("/consumerproducer");

    return 0; 
}
