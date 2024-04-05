<h1>Pierce Pickett Operating Systems Producer Consumer Problem</h1>
<bl>
This is the project that demostrates the use of shared memory and semaphores using C++. This uses two files; producer.cpp and consumer.cpp
  <br>
While making this program I decided to make a semaphore where the producer puts a random character on the buffer for the consumer to consume. It has come to my attention that the use of the rand() function may affect the output of the program which I would recommend running the program a couple of times in the case the the output isnt exactly: 
Produced: A
Produced: B
Consumed: A
Consumed: B
    <br>
    <h3>
      Compiling
    </h3>
To compile the program use a bash terminal with the files in a folder. 
to get an execution file for the two files use: 
g++ producer.cpp -pthread -o producer
g++ consumer.cpp -pthread -o consumcer
Then to run both of the execution files at the same time use:
./producer & ./consumer &
  
![Screenshot 2024-04-04 180523](https://github.com/AlwaysWorkingNeverOver/OS/assets/115910495/6840694a-5cfd-486b-9952-6ffbc48ee6ff)

![Screenshot 2024-04-04 180613](https://github.com/AlwaysWorkingNeverOver/OS/assets/115910495/606b9724-ac8c-4cf3-94e1-36bc3572f4d4)
