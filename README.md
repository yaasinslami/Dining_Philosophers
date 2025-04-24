![Philosophers](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252FWPE2MGl104V3VjxpPfeg%252Fimage.png%3Falt%3Dmedia%26token%3Dff356657-b7ce-4361-8c40-56977884a886)

# 📘 Documentation

The full documentation is posted in Medium:

👉 [View on Medium](https://medium.com/@yassinx4002/dining-philosophers-in-c-from-theory-to-practice-28582180aa37)

# Part 1: Multithreading the Dinner – Solving with Threads and Mutexes

Before we jump into the code, let’s talk briefly about **multithreading**. In modern programs, we often want to run multiple operations at the same time — that’s where threads come in. But when threads share resources (like forks in our case), things can get messy: **data races**, **deadlocks**, and **starvation** are real challenges we must handle.

The classic *Dining Philosophers* problem models exactly that. My solution uses threads to simulate each philosopher, and I’ve added careful synchronization to prevent deadlocks and ensure fair access to shared resources.

---

## 🧵 What is a Thread?

A **thread** is an execution context — all the information a CPU needs to execute a stream of instructions.

> 📖 **Example:**
> Suppose you’re reading a book, and you want to take a break but resume later exactly where you left off. You’d jot down the page number, line number, and word number — that’s your *execution context*.
>
> If your roommate does the same, she can read when you're not, and resume from where *she* stopped. You take it back later and resume from *your* place.
>
> Threads work similarly — a CPU appears to do multiple things at once by switching between tasks, each with its own execution context.

On a technical level, an execution context (and thus a thread) consists of the values of the CPU’s **registers**.

> ⚠️ **Note:** Threads are different from processes.
> A **thread** is a context of execution.
> A **process** is a collection of resources associated with a computation (e.g., memory, file descriptors, user credentials).
>
> A process can have one or many threads.

---

## 🔄 What is Multithreading?

**Multithreading** is the technique of running multiple threads in parallel — often used to improve the responsiveness or performance of programs. It allows different parts of a task (or different tasks) to run independently.

---

## ⚔️ Race Condition vs Data Race

###  Data Race

A **data race** occurs when:
- Two instructions from different threads access the **same memory location**,
- At least one of the accesses is a **write**,
- And there's **no synchronization** enforcing the order between these accesses.

The definition of a **data race** is pretty clear, and therefore, its discovery can be automated. A data race occurs when 2 instructions from different threads access the same memory location, at least one of these accesses is a write and there is no synchronization that is mandating any particular order among these accesses.

### ⚠️ Race Condition

A **race condition** is a **semantic error** — a flaw in the timing or ordering of events that causes **incorrect behavior**. Many race conditions can be caused by data races, but not always.

### 💡 Example (with `x` as a shared variable)
```
	Thread 1    Thread 2

 	lock(l)     lock(l)

	x=1         x=2

	unlock(l)   unlock(l)
```

In this example, the writes to x from thread 1 and 2 are protected by locks, therefore they are always happening in some order enforced by the order with which the locks are acquired at runtime. That is, the writes’ atomicity cannot be broken; there is always a happens-before relationship between the two writes in any execution. We just cannot know which write happens before the other a priori.

There is no fixed ordering between the writes, because locks cannot provide this. If the programs’ correctness is compromised, say when the write to x by thread 2 is followed by the write to x in thread 1, we say there is a race condition, although technically there is no data race.

It is far more useful to detect race conditions than data races; however this is also very difficult to achieve.

![data race](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252F2n1xSsUGehUOqS3CxTjW%252F1_G4hNrOnbK_KgRI35vhxk4A.png%3Falt%3Dmedia%26token%3Dee0bacf8-231d-456d-9352-1bc17ce11f83)

---

### 🧵 What is a DeadLock?

Let me explain a real world (not actually real) example for a deadlock situation from the crime movies. Imagine a criminal holds an hostage and against that, a cop also holds an hostage who is a friend of the criminal. In this case, criminal is not going to let the hostage go if cop won’t let his friend to let go. Also the cop is not going to let the friend of criminal let go, unless the criminal releases the hostage. This is an endless untrustworthy situation, because both sides are insisting the first step from each other.

![deadlock_example](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252FZJtwkiTJV2aGXJivP0cK%252F0_m6CUan9RUjx1VEoY.png%3Falt%3Dmedia%26token%3Df9db045d-2f66-43c5-87e4-9f98942d021d)

So simply, when two threads needs two different resources and each of them has the lock of the resource that the other need, it is a deadlock.
Another High Level Explanation of Deadlock : **Broken Hearts**💔
>
> You are dating with a girl and one day after an argument, both sides are heart-broken to each other and waiting for an I-am-sorry-and-I-missed-you call. In this situation, both sides want to communicate each other if and only if one of them receives an I-am-sorry call from the other. Because that neither of each is going to start communication and waiting in a passive state, both will wait for the other to start communication which ends up in a deadlock situation.
>

---

### 🧠 Understanding the Porblem

The Dining Philosophers problem is a classic synchronization challenge in computer science. It models a group of philosophers who alternate between thinking, eating, and sleeping, while sharing a limited number of forks (or resources).
In my implementation, each philosopher is represented by a thread. This is a natural fit for concurrent programming and allows clean resource sharing within a single process space.
To eat, a philosopher needs two forks — one on the left, and one on the right. The key challenge? Avoiding deadlocks and starvation while managing concurrent access to the forks.

in **setup_threads()** i create the philosophers threads, and also the supervisor monitor thread that monitoring our simulation

![](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252FaaWiUSSvgUvUK15s1LQw%252F1_BBAhHlMTEOFdbr7erUpIBg.png%3Falt%3Dmedia%26token%3D76026ffc-9071-4c71-b8da-272e16fe318e)

as u can see the philosopher() is the main function for each thread, and the monitor_func() is the monitor one.

![](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252F5zxaTpyuTPpFDAanOt8O%252F1_7GwoZwyxpf-dKm69gRHZhQ.png%3Falt%3Dmedia%26token%3D7466cea5-39a6-4553-9b8e-c8b52af844f5)

- The **philosopher()** function serves as the entry point for each philosopher thread in the Dining Philosophers simulation. It takes a pointer to a t_philo struct — containing the philosopher’s state and simulation context — and orchestrates the behavior of that individual philosopher.

-	The first condition handles a special edge case: if there’s only one philosopher participating in the simulation.

![](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252FznO3hbH3jkQentF0Ih31%252F1_hOOFZorHHAYCZVhLaAxzYA.png%3Falt%3Dmedia%26token%3D7543ba2a-57a8-4994-aaad-5affc5d83792)


- Since eating requires two forks and only one is available in this scenario, the function delegates to alone_philo(), a dedicated routine that simulates this unique case and then returns, preventing undefined behavior or deadlocks.

- Next, i implement a subtle but effective anti-deadlock mechanism.

![](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252F63eihcgqG914sPOVdNkU%252F1_icgV3gMKsKRd3C_ZPX69Eg.png%3Falt%3Dmedia%26token%3D0669082e-5094-474b-97a8-194bcaf199dd)

Philosophers with an odd ID are paused for half the time_to_eat duration before proceeding. This staggered start helps desynchronize the philosophers’ actions, ensuring that not all threads attempt to grab forks simultaneously. This technique reduces the risk of circular waiting and deadlocks, which are common pitfalls in concurrent systems like this one.

![](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252F8dxrdBZHCxbW0MSWsGaP%252F1_K9qkLuzkvfAqGUSFaWxcUQ.png%3Falt%3Dmedia%26token%3Dd1dbfd7d-1637-4462-88be-5e71db19dc94)

- Finally, each philosopher enters their main life cycle through the philo_routine function. This is where the philosopher repeatedly attempts to think, eat, and sleep according to the rules of the simulation — all while carefully synchronized with mutexes to ensure safe access to shared forks.

![](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252FfEr2Y8AYge430hYM1EoK%252F1_1oKEd2QgCdwxLDgIGdQ4cQ.png%3Falt%3Dmedia%26token%3Da7962b50-2c9c-4971-b6ef-5059f12ccae0)



- Before eating, I update the last_meal_time to reflect the exact moment the philosopher begins their meal. This value is protected with a mutex (mealtime_lock) to ensure thread-safe access — especially important for the monitoring thread that checks if any philosopher has starved.


- Once the timestamp is safely updated, the philosopher logs the action ("is eating") and enters a controlled sleep using philo_hang(), simulating the duration of the eating process. Unlike a basic sleep, philo_hang() allows more precise and responsive timing by looping with short intervals.


- After eating, I immediately check if the simulation has been flagged to stop (e.g., a philosopher has died or all have eaten enough). If so, the philosopher puts down their forks and exits early.
If the simulation is still active, the philosopher releases the forks to make them available for others. Then, using another mutex (meal_lock), I increment the meals_eaten counter safely to reflect progress toward a possible meal limit.


- Finally, a second check for should_stop ensures the simulation can terminate promptly after the meal count update. If everything is still running, the function returns 1 to indicate a successful eating cycle


- In short, this function efficiently sets up the philosopher’s behavior while proactively guarding against concurrency issues such as deadlocks, ensuring a smooth and fair simulation.


To ensure that the simulation runs reliably and terminates when it should, I also spawn a monitor thread. Its job is to continuously:


- Detect if any philosopher has starved, meaning they haven’t eaten within the allowed time_to_die window.
- Check if all philosophers have finished eating the required number of meals, if such a constraint exists.


The monitor thread is simple, efficient, and wakes up every 500 ms to do its job. Once it detects that a philosopher has died, or that all have eaten enough, it signals the simulation to stop.


Under the hood, this logic is handled in the monitor_func(). It repeatedly calls two helper functions: starved() and eat_done().

If either returns true, the monitor breaks its loop and exits.

- The starved() function checks each philosopher's last_meal_time (protected by a mutex for thread safety) to determine if too much time has passed without eating. If a philosopher is detected as dead, the simulation's state is updated accordingly by setting a stop_flag, recording the death time and philosopher ID.


- The eat_done() function checks whether every philosopher has completed the required number of meals. If so, it sets the stop_flag to stop the simulation cleanly.


To make sure that philosophers can react to this status, the shared stop_flag is safely checked using the should_stop() function, which guards access with a mutex.


Thanks to this monitor thread, the simulation remains both responsive and deterministic, ensuring proper termination whether due to starvation or successful completion.


This shared state is tracked using a stop_flag, which is protected by a mutex to avoid race conditions.


Once the simulation is complete and all threads have finished executing, we return to the main thread for final reporting.

![](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252FgQO25WGGnn7qyuD2K7fJ%252F1_Qv567fzTU9zEyx3ZKBRNxQ.png%3Falt%3Dmedia%26token%3Dfdea307e-de60-47ad-bbea-f347e4411f32)

At this point:
- The setup_threads() function has already spawned all philosopher and monitor threads and waited for them to finish using pthread_join.

- Therefore, no threads are running anymore — all have returned safely.

- As a result, accessing shared data like stop_flag, death_time, or dead_philo does not require synchronization (like mutexes), because no concurrent access can occur. This makes the final reporting clean, simple, and safe.

The program checks the reason for the simulation’s termination:

- If stop_flag == DEAD, it means a philosopher has died. We print the timestamp of the death and the ID of the philosopher.

- If stop_flag == STOP, all philosophers completed their required meals successfully, and we announce that the simulation was completed without incident.

This final output acts as a summary — a clear and direct report on how the simulation concluded. It’s a lightweight, efficient ending that gives meaningful feedback to the user or tester.


To wrap things up properly, I finish by calling free_simulation, which is my custom cleaner function. Its job is to safely destroy and free all the resources that were dynamically allocated or initialized during the simulation.

![](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252FXbq2IzyBEYMYwVO2WZ6F%252F1_aQRtSJxCHao5FWZJKQUP-w.png%3Falt%3Dmedia%26token%3D8fd4f1bd-3184-46cb-9755-d661560249fa)


To ensure robust cleanup, I use a bitmask (**simulation->mut**) to track the initialization status of each mutex. That way, if an error occurs midway through setup — for example, I succeed in initializing Mutex A and B, but Mutex C fails — I can confidently destroy only A and B, avoiding undefined behavior or double-free errors. This kind of design makes my resource management safe, modular, and error-resilient.

# Part 2: Forking the Table — Philosophers as Processes

In this version of the problem, each philosopher is no longer just a thread — they are now independent processes, each with their own memory space. This shift fundamentally changes how we manage communication and synchronization.

Unlike threads, which naturally share memory, processes are isolated from one another. That means we can’t simply use shared variables or mutexes anymore — instead, we must rely on Inter-Process Communication (IPC) mechanisms such as semaphores, shared memory, or pipes to coordinate their actions.

This design brings both benefits and challenges:

- **Improved fault isolation:** if a philosopher crashes, it won’t corrupt the memory of the others.
- **More complex synchronization:** since memory isn’t shared, we have to explicitly set up communication channels.

The core goals remain the same — prevent data corruption, ensure fairness, avoid deadlocks and starvation — but the tools and approach are now different.

In the following sections, I’ll walk you through how I structured this version: from process creation and monitoring, to using named semaphores to safely manage forks and state updates.

When we move from threads to processes, the rules of the game change. Let’s explore the essential building blocks that make this work:

### 🧵 Processes vs Threads

- First thing you need to know to understand the difference between a process and a thread, is a fact, that ***processes do not run, threads do***.
- So, what is a thread? Closest I can get explaining it is an execution state, as in: a combination of CPU registers, stack, the lot. You can see a proof of that, by breaking in a debugger at any given moment. What do you see? A call stack, a set of registers. That’s pretty much it. That’s the thread.
- Now, then, what is a process. Well, it’s a like an abstract “container” entity for running threads. As far as OS is concerned in a first approximation, it’s an entity OS allocates some VM to, assigns some system resources to (like file handles, network sockets), &c.
- How do they work together? The OS creates a “process” by reserving some resources to it, and starting a “main” thread. That thread then can spawn more threads. Those are the threads in one process. They more or less can share those resources one way or another (say, locking might be needed for them not to spoil the fun for others &c). From there on, OS is normally responsible for maintaining those threads “inside” that VM (detecting and preventing attempts to access memory which doesn’t “belong” to that process), providing some type of scheduling those threads, so that they can run “one-after-another-and-not-just-one-all-the-time”.
- In this project, each **philosopher becomes its own process**, meaning they can’t just access shared variables or mutexes — they need special tools to talk to each other and coordinate.
---

### 📡 Inter-Process Communication (IPC)

Since processes are isolated, we need ways to allow them to communicate and synchronize. This is where IPC (Inter-Process Communication) comes in.

Inter process communication is a mechanism which allows different processes to communicate and synchronize shared actions by using message passing and shared memory.

Inter process communication can be achieved by

1. Shared memory using standard system functions like shmget(), shmat(), etc.
2. fifo (named pipe)
3. pipe (unnamed pipe)

It is very important to synchronize shared data items and processes in communication to avoid problems like deadlock, starvation, etc.

anyway , I use semaphores to manage shared resources like forks and logs.

---

### 🚦 Semaphores

A **semaphore** is a special counter used to control access to a shared resource. Think of it like a traffic light:

Think of semaphores as bouncers at a nightclub. There are a dedicated number of people that are allowed in the club at once. If the club is full no one is allowed to enter, but as soon as one person leaves another person might enter.

It’s simply a way to limit the number of consumers for a specific resource. For example, to limit the number of simultaneous calls to a database in an application.

Here is a very pedagogic example in C:

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_GUESTS 50
#define CLUB_CAPACITY 3

sem_t bouncer;

void* guest(void* arg)
{
    int id = *(int*)arg;

    printf("Guest %d is waiting to enter the nightclub.\n", id);
    sem_wait(&bouncer); // Wait to enter (acquire semaphore)

    printf("Guest %d is doing some dancing 🕺.\n", id);
    usleep(500000); // Sleep 0.5 sec to simulate dancing

    printf("Guest %d is leaving the nightclub.\n", id);
    sem_post(&bouncer); // Release (leave club)

    free(arg);
    return NULL;
}

int main()
{
    pthread_t threads[MAX_GUESTS];
    sem_init(&bouncer, 0, CLUB_CAPACITY); // Init with 3 available slots

    for (int i = 0; i < MAX_GUESTS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;

        if (pthread_create(&threads[i], NULL, guest, id) != 0) {
            perror("Failed to create thread");
            free(id);
        }
    }

    for (int i = 0; i < MAX_GUESTS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&bouncer);
    return 0;
}
```

### 🚪 Stepping into the Implementation — How the Philosophers Eat, Safely

Now that we’ve covered the concepts, it’s time to see them in action.

In this version of the project, **each philosopher is represented by a separate process.** That means they don’t share memory the way threads do — so to coordinate access to shared resources (like forks or logs), we use inter-process communication, specifically **named semaphores.**

Here’s the big idea:

- 🪑 We create a semaphore called /forks, initialized to the number of philosophers.
This semaphore represents the total number of available forks on the table.

- When a philosopher wants to eat, they wait (sem_wait) twice on the /forks semaphore — once for each fork. This is like saying: “I won’t eat until I’m sure I can grab two forks safely.”

- 🍽️ After eating, the philosopher posts (sem_post) twice, releasing the two forks back to the table so another philosopher can use them.

- 🧾 To prevent messy console logs (from multiple processes printing at once), we use a /log_sem semaphore. A philosopher must acquire it before printing and release it afterward.

This simple but powerful strategy ensures:

- No two philosophers grab the same fork at the same time.
- Output is clean and readable.
- And, combined with a monitoring mechanism, we can detect when someone has starved or when all have eaten enough.

Before the simulation starts, I set up all the semaphores that will coordinate the philosophers (processes). This is done in **initialize_semaphores.**

Let me walk you through what each semaphore does and why it matters:

#### 🥢 FORK_SEM
- This semaphore represents the total number of forks on the table.
- It’s initialized to the number of philosophers (since there’s one fork per philosopher).
- Each philosopher needs to successfully wait (decrement) twice on it to simulate taking two forks before eating, and post (increment) twice after eating to release them.

#### 🧾 LOG_SEM
- Shared output from multiple processes can get messy.
- So I use this semaphore as a mutex for printing logs.
- Only one process can acquire it at a time, ensuring clean and readable console output.

#### DONE_SEM
- This one is a little more interesting. It’s used to track progress toward the meal goal.
- If the number_of_eats argument is specified, then each philosopher will post to DONE_SEM once they've finished eating enough times.
- Another process (the monitor or parent) can then wait on this semaphore as many times as there are philosophers, to know when the simulation can stop.

#### END_SEM
- This semaphore acts as a global stop signal.
- Once the simulation needs to terminate — either because someone died or everyone is done — the controller process posts to this semaphore.
- Each philosopher periodically checks END_SEM and exits cleanly when it's signaled.

>
> Note: I always sem_unlink semaphores before and after opening them. This ensures that any remnants from previous runs don’t interfere with the current one.
>

With all four semaphores correctly initialized, we now have a robust communication and synchronization backbone for our philosopher processes.

Next, we’ll see how the processes are created and how each philosopher uses these semaphores during the simulation.

---

![](https://miro.medium.com/v2/resize:fit:1400/format:webp/1*gmJJ88vQMZuDv_2mpaF3zw.png)

![](https://miro.medium.com/v2/resize:fit:1400/format:webp/1*Ypxj5Q5tXry9nxnjqcPswg.png)

Each philosopher is a **separate process** forked from the main program. The synchronization and logic are handled via:

- **Named semaphores** for inter-process communication.
- **Threads** inside each philosopher process to monitor their own state (death detection).
- **Global simulation control** using semaphores like done_sem and end_sem.
-
i use done_monitor() to create the supervisor monitor thread (in main_thread), and detach it to run independently.

![](https://miro.medium.com/v2/resize:fit:1400/format:webp/1*s6yXV2cMiGdSHVHNYfcL0Q.png)

so as i say , he monitor the meal_eaten by using a done_sem semaphore by waiting for all philosophers (waiting to each philo post that semaphore when he eat at least number_of_eats), when i passed that while so im sure that each philo eat at least that numbers of eats, i wait the log_sem to prevent any process from printing actions after that, i annonce the simulation done msg and post the end_sem semaphore to indicate the end of my simulation.

- you may asking, what if before the simulation_done by eating all meals philos need, a philo is died , the simulation will hang waiting for the semaphores to be posted . So here where the exit_thread come:

![](https://miro.medium.com/v2/resize:fit:1212/format:webp/1*-D0xG1yUuRyAq5n5u4PAPA.png)

- when a philo is died, on other wait_philos() function , i catch that exit from that philo and post the end_sem semaphore then i call kill_world() to finish the program, and that’s why i use all this stuff.

Let’s move on to the next.

now i fork for each philo and calling child_philo() function that control the life of each one:

![](https://miro.medium.com/v2/resize:fit:1400/format:webp/1*TTPGRBb2a8YwsVDKV1Ei7A.png)

i use meal_sem() that create two semaphores for each process:

- philo->meal_sem : which protege the meal eaten count for each philo
- philo->mealtime_sem: which protege the last_meal_time as we see in the first part.

![](https://miro.medium.com/v2/resize:fit:1032/format:webp/1*1Q0L-VSEZOsPknLa9N5dSg.png)

this is the semaphores opened by the process 1, so processes has their own meal and mealtime semaphores.

same concept of part1, the eating, update last_meal_time and meals_eaten with safe protection by meal_semx and mealtime_semx

- after eating:

![](https://miro.medium.com/v2/resize:fit:1276/format:webp/1*uQOnjtGYC_dnO8cZykck-Q.png)

- i check if the philo achieved the number of meals should eaten, if true;
- i post the done_sem to noticed the supervisor thread as i show you previously.

let’s back to the process’s monitor thread:

![](https://miro.medium.com/v2/resize:fit:1400/format:webp/1*wBxsDgzFkOTkHlnWL-zBsw.png)

- if the monitor detect death of a philo, i print a death msg and exit(1).
- that exited value will be catched by wait_philos():

![](https://miro.medium.com/v2/resize:fit:1196/format:webp/1*aPsf_Cy2NTx26n2FjsZdxg.png)

in the parent process, after all philosophers are forked:

- The parent calls wait_philos(sim);
- This function **waits for any child process** to finish using waitpid(-1, &status, 0); here i pass -1 instead of the pid of each philo, to include all processes that have the same GID of the parent process
- Each time a child terminates, `handle_status` is called to **check how the child exited.**

![](https://miro.medium.com/v2/resize:fit:1400/format:webp/1*IZt5wzqPbalDckxuDyAL_A.png)

- The parent monitors philosopher death or abnormal exit using two checks:
- Checks if the philosopher exited normally:
- If exit code is 1, the philosopher died (e.g., from starvation).
- Signals that the simulation should end by posting on end_sem.
- Checks if the philosopher exited in abnormal way:
- This means the child was killed by a signal (e.g., SIGKILL, SIGTERM, etc.).
- so return 1 to kill_world
in summurized way:
- If any of the above checks return true.

1. handle_status returns 1.
2. wait_philos breaks the waiting loop.
3. kill_world(sim); is called to:
- Kill all remaining philosopher processes.
- Clean up shared semaphores and memory.

```
┬───────────────
| Parent forks  │
│ philosophers  │
└─────┬─────────┘
      │
      ▼
┌──────────────────────────────────┐
│ Parent calls wait_philos(sim)   │
└─────┬────────────────────────────┘
      │
      ▼
┌──────────────────────────────────┐
│ waitpid waits for any child     │
│ to exit (loop)                  │
└─────┬────────────────────────────┘
      ▼
┌────────────────────────────┐
│ handle_status is called    │
│                            │
│ if (exit code != 0) or     │
│    (killed by signal)      │
└─────┬──────────────────────┘
      ▼
┌────────────────────────────┐
│ post(end_sem)              │
│ return 1 → break loop      │
└─────┬──────────────────────┘
      ▼
┌────────────────────────────┐
│ kill_world(sim);           │
│ cleanup + kill all philos  │
└────────────────────────────┘
```