2.	int use  = 0;
        int fill =0; 

Consider the below scenario:
•	Two producers threads both call put() at the same time. Producer thread1 runs first and starts to fill the buffer entry (fill = 0 and buffer[fill = value.]). Before the producer thread1 gets a chance to increment the fill counter to 1, producer thread2 starts to run and the same line of code, it also puts its data into 0th element of buffer, which means that the old data is overwritten.
•	Similarly, with the consumers, 2 consumer threads will read the same value twice, as consumer thread1 will not have decremented the use counter before consumer thread2 reads the data.

       
3.Because of the race condition described in the previous answer, 2 consumer threads read the same value from the buffer. The above command prints all such duplicate reads from the threads.

4.Since both producer and consumer wait for the same mutex lock to be released.
Consumer locks the mutex, it waits for the ‘full’ semaphore to be released by producer.  And since consumer has locked the mutex, producer can’t go ahead to clear the ‘full’ semaphore.
Hence having 2 mutex locks will solve the problem.

