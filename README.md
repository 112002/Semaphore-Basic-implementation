# Semaphore-Basic-implementation
Semaphores are integer variables that are used to solve the critical section problem by using two atomic operations, wait and signal that are used for process synchronization.

The definitions of wait and signal are as follows −

- Wait
The wait operation decrements the value of its argument S, if it is positive. If S is negative or zero, then no operation is performed.

- Signal
The signal operation increments the value of its argument S
