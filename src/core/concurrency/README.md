# Concurrency

| Example             | Topic                                                          |
|---------------------|----------------------------------------------------------------|
| `ThreadManagement`  | `join`, `detach`, `joinable`, RAII thread guards, `std::jthread` |
| `SharingData`       | passing arguments to threads, `std::ref`, lambda captures, `thread_local` |
| `RaceCondition`     | data races and lost updates, `std::mutex`, `std::atomic`        |
| `ConditionVariable` | waiting for a condition, producer/consumer queue               |
| `FuturePromise`     | `std::async`, `std::promise`, `std::packaged_task`, exceptions across threads |
| `Timing`            | `<chrono>` clocks and durations, measuring elapsed time        |
| `Atomic` | atomic counters and flags, compare_exchange, memory orders and std::atomic_ref *(draft)* |
| `SharedMutex` | shared_mutex, scoped_lock, unique_lock options, recursive locks and std::call_once *(draft)* |
| `Synchronization` | std::latch, std::barrier and std::counting_semaphore as coordination primitives *(draft)* |
| `Coroutine` | co_await, co_yield, co_return, the promise type, and std::generator *(draft)* |
| `ParallelAlgorithm` | execution policies: par, par_unseq, when parallelism pays off and what it costs *(draft)* |

## 1. Threads

```cpp
std::thread worker(function, arguments...);   // starts immediately
worker.join();                                // wait for it to finish
```

- A `std::thread` that is still joinable when it is destroyed calls `std::terminate()`. Join it on
  every path, or use `std::jthread` (C++20), which joins automatically.
- Arguments are **copied** into the thread. Use `std::ref(x)` to pass a reference.
- `detach()` is rarely right: nobody can wait for the thread anymore.

## 2. Synchronization

| Tool                        | Use it for                                          |
|-----------------------------|-----------------------------------------------------|
| `std::mutex` + `std::lock_guard` / `std::scoped_lock` | protecting shared data (a critical section) |
| `std::unique_lock`          | a lock that can be released early; needed by condition variables |
| `std::atomic<T>`            | single counters and flags without a lock           |
| `std::condition_variable`   | sleeping until another thread signals a change      |
| `std::future` / `std::promise` | handing one result (or exception) to another thread |

A **data race** happens when two threads access the same memory, at least one of them writes, and
nothing synchronizes them. A data race is undefined behavior, not just a wrong result. Build with
`-fsanitize=thread` (ThreadSanitizer) to find races.

## 3. Deadlocks

A deadlock happens when two threads each hold a lock and wait for the other's. To avoid them:

- lock several mutexes at once with `std::scoped_lock lock(a, b);`
- always lock mutexes in the same order
- never call unknown code (callbacks) while holding a lock
