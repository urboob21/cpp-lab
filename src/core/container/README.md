# Containers

| Example | Container |
|---|---|
| `sequence/Array` | `std::array` - fixed size, on the stack |
| `sequence/Vector` | `std::vector` - dynamic array, growth and invalidation |
| `sequence/Deque` | `std::deque` - fast at both ends, stable references |
| `associative/Set` | `std::set` - sorted unique keys, ranges, `std::multiset` |
| `unordered/UnorderedMap` | `std::unordered_map` - hash table, lookup pitfalls |
| `adapter/Stack` | `std::stack` - LIFO |
| `adapter/Queue` | `std::queue` - FIFO |
| `adapter/PriorityQueue` | `std::priority_queue` - heap, custom priority |

Choosing a container:

```
Need key -> value?          no  -> order matters and you insert/erase in the middle? -> std::list
                                   only at the ends?                                 -> std::deque
                                   otherwise                                         -> std::vector
                            yes -> need sorted iteration? -> std::map / std::set
                                   only fast lookup?      -> std::unordered_map / _set
```

`std::vector` is the default: contiguous memory keeps the CPU cache happy, which usually beats the
theoretical advantages of other containers.

---
# Containers library
- The Containers library is a generic collection of class templates and algorithms that allow programmers to easilly implement common data structures.
- All container functions can be called concurrently by different threads on different containers
## 1. Sequence Containers
*Sequence containers* implement data structures which can be accessed sequentially.

|name|description|
|---|---|
|array|fixed-sized inplace contiguous array|
|vector|resizeable contiguous array|
|dequeue|double-ended queue|
|forward_list|singly-linked list|
|list|doubly-linked list|

<br>

## 2. Unordered Containers
*Unordered associative containers* implement unsorted (hashed) data structures that can be quickly searched (**O(1) average, O(n) worst-case complexity**).

|name|description|
|---|---|
|unordered_map|collection of key-value pairs, hashed by keys, keys are unique|
|unordered_set|collection of unique keys, hashed by keys|

<br>

## 3. Adapter Containers
*Container adaptors* provide a different interface for **sequential containers**.

|name|description|
|---|---|
|queue|apdapts a container to provide queue (**FIFO** data structure)|
|stack|adapts a container to provide stack (**LIFO** data structure)|

<br>

## 4. Associative Containers
- *Associative containers* implement sorted data structures that can be quickly searched (**O(logn)**)

|name|description|
|---|---|
|set|collection of unique keys, sorted by keys|
|map|collection of key-value pairs, sorted by keys|

<br>
