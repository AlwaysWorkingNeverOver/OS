#ifndef SHARED_HPP
#define SHARED_HPP

#include <semaphore.h>
#include <mutex>
#include<thread>

extern sem_t* empty;
extern sem_t* full;
extern std::mutex buffer_mutex;
extern int* buffer;
extern int in;

void producer();
void consumer();
#endif
