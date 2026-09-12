# Smart pointers

| Example  | Topic                                                          |
|----------|----------------------------------------------------------------|
| `Unique` | exclusive ownership, moving, sink functions, arrays, custom deleters |
| `Shared` | reference counting, `shared_ptr<const T>`, `enable_shared_from_this` |
| `Weak`   | observing without owning, `lock()`, breaking reference cycles  |

Smart pointers apply RAII to heap memory: the destructor releases the object, so there is no
`delete` to forget and no leak when an exception is thrown.

## 1. Which one?

```
Does anybody need to share ownership?
├── no  -> std::unique_ptr<T>   (the default: zero overhead, move-only)
└── yes -> std::shared_ptr<T>   (reference counted)
             └── observers that must not keep it alive -> std::weak_ptr<T>
```

Raw pointers and references are still fine for **non-owning** access: `void draw(const Shape&)`.

## 2. Function signatures express ownership

| Parameter                     | Meaning                                    |
|-------------------------------|--------------------------------------------|
| `void f(T&)` / `void f(T*)`   | uses the object, does not own it           |
| `void f(std::unique_ptr<T>)`  | takes ownership ("sink")                   |
| `void f(std::shared_ptr<T>)`  | becomes one more owner                     |
| `std::unique_ptr<T> make()`   | factory: hands ownership to the caller     |

## 3. Pitfalls

- Create smart pointers with `std::make_unique` / `std::make_shared`, never
  `std::shared_ptr<T>(this)` (use `enable_shared_from_this`).
- Two `shared_ptr`s that own each other form a **cycle** and are never freed. Make one direction
  `weak_ptr`.
- `shared_ptr` makes the *reference count* thread-safe, not the object it points to.
- `unique_ptr::release()` gives up ownership without deleting. You usually want `reset()`.
