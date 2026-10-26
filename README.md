_This project has been created as part of the 42 curriculum by nfitahin_

# Codexion

## Description

**Codexion** is a concurrent simulation written in C. Several **coders** (one POSIX thread each) sit in a circle. Between two neighbouring coders there is one **dongle**, so there are as many dongles as coders. To compile, a coder needs **two dongles** (the one on its left and the one on its right). After a compile, a coder debugs, then refactors, then tries to compile again.

Rules of the simulation:

- A coder who does not **start** a compile within `time_to_burnout` ms (counted from the start of its previous compile, or from the start of the simulation) **burns out**, and the simulation stops.
- When a dongle is released, it cannot be taken again before `dongle_cooldown` ms have passed.
- The simulation also stops, without any burnout, when every coder has compiled `number_of_compiles_required` times.
- When several coders want the same dongle, a **scheduler** decides who is served first: **FIFO** (first request first) or **EDF** (closest burnout deadline first).

Every state change is printed as `<timestamp_ms> <coder_id> <message>`.

---

## Instructions

### Compilation

```bash
make
```

The Makefile compiles the sources in `src/` with `-Wall -Wextra -Werror -pthread` and produces the binary `codexion`. Other rules: `make clean`, `make fclean`, `make re`.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Meaning | Accepted values |
|---|---|---|
| `number_of_coders` | Number of coders (= number of dongles) | integer > 0 |
| `time_to_burnout` | Max ms without starting a compile | integer > 0 |
| `time_to_compile` | Duration of a compile (ms) | integer >= 0 |
| `time_to_debug` | Duration of debugging (ms) | integer >= 0 |
| `time_to_refactor` | Duration of refactoring (ms) | integer >= 0 |
| `number_of_compiles_required` | Compiles each coder must do | integer > 0 |
| `dongle_cooldown` | Cooldown after a dongle is released (ms) | integer >= 0 |
| `scheduler` | Scheduling policy | `fifo` or `edf` |

All numbers must be plain integers that fit in an `int`. Wrong argument count, non-numeric values, values out of range or an unknown scheduler print an error on `stderr`, and the program exits with code 1.

### Examples

```bash
./codexion 1 800 200 200 200 10 0 fifo     # one dongle only: the coder burns out at ~800 ms
./codexion 5 2000 200 200 200 10 0 fifo    # no burnout, stops when everybody compiled 10 times
./codexion 5 2000 200 200 200 7 0 edf      # same with the EDF scheduler
./codexion 5 500 200 200 200 10 0 fifo     # impossible (a cycle lasts 600 ms > 500 ms): burnout
```

### Log messages

`has taken a dongle` (printed twice, once for each dongle), `is compiling`, `is debugging`, `is refactoring`, `burned out`.

---

## How the code works

### Structures

- `t_env`: shared state (list of coders, list of dongles, start time, `is_running` flag, two mutexes). It is created on the stack in `main`. **There is no global variable.**
- `t_coder`: id, its two dongles (`left_dongle`, `right_dongle`), `time_burnout` (start of its last compile), `code_compiled`, `have_finished`, and its own `state_lock`.
- `t_dongle`: id, `is_available`, `available_at` (end of cooldown), its own `state_lock`, and a waiting queue `t_heap queue`.

### Threads

- One thread per coder (`pthread_create`), each running `ft_coder_routine`.
- One **monitor** thread running `ft_monitor_routine`. The main thread waits for it and for all coders with `pthread_join` (no detached thread).
- Even-numbered coders sleep 1 ms at the start so that all coders do not rush to the dongles at the same instant.

### The schedulers: one min-heap per dongle

FIFO and EDF use the **same data structure**: each dongle owns a binary min-heap (`t_heap`, a dynamic array of `t_heap_entry`) containing the coders waiting for it. The coder at the top of the heap is the one served first. The two policies differ only by the **priority number** given to a coder when it enters the heap (`ft_heap_push`). The smallest number wins.

| Scheduler | Priority number | Effect |
|---|---|---|
| `fifo` | `next_seq`, a counter of the dongle's heap that goes up by 1 at each push | The coder who asked first has the smallest number and is served first. |
| `edf` | `time_burnout + time_to_burnout`, the absolute time of the coder's burnout deadline | The coder closest to burnout is served first. |

Tie-breaking (`has_higher_priority`): smaller priority first, then smaller `seq` (older request), then smaller `coder_id`.

Complexity: `ft_heap_push` and `ft_heap_pop` are O(log N), `ft_heap_peek` is O(1). The heap capacity is `number_of_coders`, because a coder is at most once in a given queue.

### Taking the dongles (`ft_acquire_dongles`)

1. **Register** (`ft_register`): the coder locks its two dongles, then pushes itself in the queue of each dongle, and unlocks.
2. **Try** (`ft_try_acquire`), in a loop: the coder locks its two dongles and checks, for **both** of them, that it is at the **top of the queue**, that the dongle **is available**, and that the **cooldown is over** (`now >= available_at`).
   - If everything is true, it removes itself from both queues, marks both dongles as not available, and prints `has taken a dongle` twice.
   - Otherwise it takes **nothing**, unlocks, sleeps `usleep(200)` and tries again.
3. After compiling, the coder releases both dongles (`ft_release_dongle`): `is_available = 1` and `available_at = now + dongle_cooldown`.

### Burnout

`time_burnout` is set at the start of each compile (`ft_set_burnout`). The monitor wakes up every 100 microseconds and checks every coder that is not finished: if `now - time_burnout > time_to_burnout`, it sets `is_running = 0` and prints `burned out`. The burnout is therefore detected within about 1 ms (the clock has a 1 ms resolution). If all coders are finished, the monitor also sets `is_running = 0`.

All waits (`ft_sleep`, waiting for dongles) check `is_running`, so all threads stop quickly.

---

## Blocking cases handled

### 1. Deadlock

A deadlock needs a circular wait. Two things prevent it:

- **Fixed locking order.** Every coder locks its **lower-numbered dongle first**, then the higher one (`ft_assign_dongles`: `left_dongle` is always the dongle with the smaller id, so the last coder takes dongle 0 before dongle N-1). Because every thread locks mutexes in the same global order, there is no circular wait between mutexes.
- **All or nothing.** A coder never keeps one dongle while waiting for the other. It takes both dongles in one step, under both locks, or none of them. The mutexes are only held for a very short time and are **never held during compile, debug or refactor**. There is no `trylock`: the coder waits by checking the queue and sleeping a short time.

### 2. Starvation

- With `fifo`, the dongle is given to the coder who asked first, so nobody can be overtaken forever.
- With `edf`, the coder closest to burnout is served first, which protects the coders who are about to die.

Note: a coder is served only when it is at the top of the queue of **both** its dongles.

### 3. Cooldown

When a dongle is released, `available_at` is set to `now + dongle_cooldown`. A coder who is first in the queue but arrives before the end of the cooldown does not take the dongle and tries again 200 microseconds later.

### 4. Single coder

With one coder there is only one dongle and a coder needs two. The coder never compiles and waits until the simulation stops. The monitor detects the burnout at `time_to_burnout`.

### 5. Precise burnout detection

The monitor is a separate thread that checks every 100 microseconds. The deadline is checked only against coders that are not finished, so a coder that completed all its compiles never causes a false burnout.

### 6. Log serialization

Every `printf` is done under `print_mutex`. The two `has taken a dongle` lines of a coder are printed under the **same lock**, so they stay together, and lines of different coders are never mixed inside a line.

### 7. Memory

Heaps (`malloc`), dongles, coders and the thread array are all freed at the end (`ft_clean_simulation`, `ft_dongles_free`, `ft_coders_free`).

---

## Thread synchronization mechanisms

Only `pthread_mutex_t` is used (no condition variable, no semaphore). Waiting is done by **polling**: the thread checks a condition and calls `usleep`.

| Mutex | Protects |
|---|---|
| `env->print_mutex` | All output lines (`printf`) |
| `env->state_mutex` | The `is_running` flag (read by all threads, written by the monitor) |
| `coder->state_lock` | `time_burnout` and `have_finished` of this coder (written by the coder, read by the monitor) |
| `dongle->state_lock` | `is_available`, `available_at` and the waiting queue (heap) of this dongle |

Why each one is needed:

- `is_running` is read by every coder and written by the monitor, so it needs `state_mutex`.
- `time_burnout` and `have_finished` are written by a coder and read by the monitor at the same time, so each coder has its own `state_lock`.
- A dongle is shared by two neighbour coders (availability, cooldown and waiting queue), so each dongle has its own `state_lock`. Having one mutex per dongle (instead of one big mutex) lets coders that do not share a dongle work in parallel.
- `print_mutex` makes sure that two coders never print at the same time.

The code was checked with `-fsanitize=thread` and `-fsanitize=address` (no data race, no leak reported).

---

## Resources

### Documentation and references

- [POSIX Threads Programming (LLNL)](https://hpc-tutorials.llnl.gov/posix/): mutexes and threads.
- [The Little Book of Semaphores](https://greenteapress.com/wp/semaphores/): concurrency problems (Dining Philosophers).
- [Helgrind manual](https://valgrind.org/docs/manual/hg-manual.html): detecting data races.
- `man pthread_create`, `man pthread_mutex_lock`, `man gettimeofday`, `man usleep`.

### Use of AI

AI tools were used as helpers, not to design the scheduling logic:

- **Refactoring and norm:** splitting long functions into smaller ones (heap shift-down, coder actions) to respect the 25-line and 80-column limits of the 42 norm, and checking that lock / unlock pairs stayed correct.
- **Review:** checking the code for data races and memory leaks with sanitizers, and checking that this README describes what the code really does.
- **Explanations:** preparing the defense (explaining the code and the scheduling idea).
