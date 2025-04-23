![Philosophers](https://www.gitbook.com/cdn-cgi/image/width=760,dpr=2,format=auto/https%3A%2F%2Ffiles.gitbook.com%2Fv0%2Fb%2Fgitbook-x-prod.appspot.com%2Fo%2Fspaces%252FHFw3P5bHQLbZglyiVksk%252Fuploads%252FWPE2MGl104V3VjxpPfeg%252Fimage.png%3Falt%3Dmedia%26token%3Dff356657-b7ce-4361-8c40-56977884a886)

# 📘 Documentation

The full documentation is hosted on GitBook:

👉 [View on GitBook](https://slamis-organization.gitbook.io/yslami/dining-philosophers)

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