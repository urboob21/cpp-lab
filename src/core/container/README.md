# Containers

| Example | Container |
|---|---|
| `sequence/Array` | `std::array` - fixed size, on the stack |
| `sequence/Vector` | `std::vector` - dynamic array, growth and invalidation |
| `sequence/Deque` | `std::deque` - fast at both ends, stable references |
| `sequence/List` | std::list: doubly linked storage, splice, stable iterators and when not to use it *(draft)* |
| `sequence/ForwardList` | std::forward_list: a singly linked list with before_begin and insert_after *(draft)* |
| `associative/Set` | `std::set` - sorted unique keys, ranges, `std::multiset` |
| `associative/Map` | std::map: sorted keys, operator[] vs at vs insert vs try_emplace, lower_bound, custom comparator *(draft)* |
| `associative/MultiMap` | duplicate keys: equal_range, count and erasing one of several equal entries *(draft)* |
| `unordered/UnorderedMap` | `std::unordered_map` - hash table, lookup pitfalls |
| `unordered/UnorderedSet` | hash sets: custom hash and equality, load factor, and hashing a user-defined type *(draft)* |
| `adapter/Stack` | `std::stack` - LIFO |
| `adapter/Queue` | `std::queue` - FIFO |
| `adapter/PriorityQueue` | `std::priority_queue` - heap, custom priority |
| `adapter/FlatMap` | sorted vectors behind a map interface: fast lookup and iteration, slow insertion *(draft)* |

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
