# Classes

| Example | Topic |
|---|---|
| `Encapsulation` | private data, invariants, getters and setters |
| `Constructor` | initializer lists, default/delegating/copy/move, `explicit` |
| `Destructor` | destruction order, virtual destructors, RAII, stack unwinding |
| `ShallowDeepCopying` | why copying a pointer member is not copying the data |
| `RuleOfThreeFiveZero` | which special member functions to write, and when to write none |
| `Relationship` | composition, aggregation, association, dependency, nesting |
| `Friend` | friend functions, friend members, friend classes |
| `Binding` | early (static) vs late (virtual) binding, hiding vs overriding |
| `VirtualBase` | the diamond problem and virtual inheritance |

---
# Classes
- **A class** is a user-defined blueprint used to create objects. It defines the properties and behaviors that all objects of that type share.
- **An object** is an instance of a class. It represents a real entity and contains actual values for the class’s attributes.
- **An instance** is a specific object created from a class. (In practice, “object” and “instance” are often used interchangeably.)

- **Four Pillars of OOP in C++:**
  - **Abstraction** is the process of hiding the implementation details and only showing the essential details or features to the user. It allows to focus on what an object does rather than how it does it. It is achieved using abstract classes (classes that have at least one pure virtual function).
  - **Encapsulation** is the process of bundling data and methods into a single unit (class) and restricting direct access to some components. Data is hidden and accessed through public methods. It is achieved using access specifiers like private, protected, and public.
  - **Inheritance** is a mechanism where a derived class acquires the properties and behaviors of a base class, forming an is-a relationship. It improves code reuse and extensibility. It is implemented using : followed by an access specifier public, private, protected.
  - **Polymorphism** means many forms. It allows the same interface (function or method) to behave differently depending on the context. 
    - It is achieved through: 
      - **Compile-time polymorphism**: function overloading, operator overloading 
      - **Runtime polymorphism**: virtual functions

## 1. Friend
- Friend declaration (classes and functions) allows certain classes or functions to access the private and protected members of another class.

### 1.1. Non Member Function
- It serves the same kind of role as the package access specifier in Java.
- `a << b` C++ tries in this order:
```cpp
a.operator<<(b)
operator<<(a, b) (non-member function)

X OP Y
operator OP (type_of_X, type_of_Y)
```

### 1.2. Member of Another Class:
- It give access to only once specific function.
### 1.3. Class
- Friend Class : I trust this class to see my private data
- It give access to all member functions of another class.

## 2. Class Relationship
### 2.1. Composition
- **Part-of, strong ownership**
- The member is part of the class
- The member can only belong to one class at a time
- The member has its existence managed by the class
- The member does not know about the existence of the class

- UML:
```cpp
+------------------------+
|          Car           |
+------------------------+
| - engine : Engine      |
+------------------------+
            ♦
            │
            │
+------------------------+
|         Engine         |
+------------------------+
| + Engine()             |
| + ~Engine()            |
+------------------------+

class Engine
{
public:
    Engine() { std::cout << "Engine created\n"; }
    ~Engine() { std::cout << "Engine destroyed\n"; }
};

class Car
{
private:
    Engine engine; // composition

public:
    Car() { std::cout << "Car created\n"; }
    ~Car() { std::cout << "Car destroyed\n"; }
};
```

### 2.2. Aggregations
- **Has-a, weak ownership**
- The member is part of the class
- The member can belong to more than one class at a time
- The member does not have its existence managed by the class
- The member does not know about the existence of the class

- UML:
```cpp
+-----------------------------+
|        Department           |
+-----------------------------+
| - teacher : Teacher*        |
+-----------------------------+
            ◇
            │
            │
+-----------------------------+
|           Teacher           |
+-----------------------------+
| - name : std::string        |
+-----------------------------+

class Teacher
{
public:
    std::string name;
    Teacher(const std::string& n) : name(n) {}
};

class Department
{
private:
    Teacher* teacher; // aggregation

public:
    Department(Teacher* t) : teacher(t) {}
};
```

### 2.3. Associations 
- **Uses-a, loose relationship**
- The associated member is otherwise unrelated to the class
- The associated member can belong to more than one class at a time
- The associated member does not have its existence managed by the class
- The associated member may or may not know about the existence of the class
- UML
```cpp
+---------------------+                 +---------------------+
|       Doctor        |-----------------|       Patient       |
+---------------------+                 +---------------------+
| + treat(p:Patient&) |                 | - name : string     |
+---------------------+                 +---------------------+

class Patient
{
public:
    std::string name;
};

class Doctor
{
public:
    void treat(Patient& p)
    {
        std::cout << "Treating " << p.name << "\n";
    }
};
```

### 2.4. Dependency
- **Denpends-on**
-  One class uses another class to perform a task. 
-  It is temporarily created, used, and then destroyed, or passed into a member function from an external source.
- UML
```cpp
+---------------------+
|         Car         |
+---------------------+
| + start()           |
+---------------------+
           - - - - - - - - - - - - - - - - - - >
                           +---------------------+
                           |       Logger        |
                           +---------------------+
                           | + log(msg:string)   |
                           +---------------------+
class Logger
{
public:
    void log(const std::string& msg)
    {
        std::cout << msg << std::endl;
    }
};

class Car
{
public:
    void start()
    {
        Logger logger;  // dependency
        logger.log("Car started");
    }
};

```
### 2.5. Container
-  The class one class provides a container to hold multiple objects of another type
- UML
```cpp
#include <vector>

+-----------------------------+
|          Library1           |
+-----------------------------+
| - books : vector<string>    |
+-----------------------------+
              ♦
              │
              │
+-----------------------------+
|          string             |
+-----------------------------+
class Library1
{
private:
    std::vector<std::string> books; // copy values
};

+-----------------------------+
|          Library2           |
+-----------------------------+
| - teachers : vector<T*>     |
+-----------------------------+
              ◇
              │
              │
+-----------------------------+
|           Teacher           |
+-----------------------------+
class Library2
{
private:
    std::vector<Teacher*> teachers; // store pointers
};

```

## 2.6. Inheritance
- **Is-a**
```cpp
+----------------------+
|        Animal        |
+----------------------+
| + eat()              |
+----------------------+
           △
           │
           │
+----------------------+
|         Dog          |
+----------------------+
| + bark()             |
+----------------------+
class Animal
{
public:
    void eat() { std::cout << "Eating\n"; }
};

class Dog : public Animal
{
public:
    void bark() { std::cout << "Woof\n"; }
};
```

## 2.7 Embedded a.k.a Nested/Inner Class
- Type-level containment
```cpp
+----------------------+
|        Car           |
+----------------------+
|                      |
+----------------------+
           (+)
           │
           │
+----------------------+
|         Engine       |
+----------------------+
| + start()            |
+----------------------+

class Car
{
public:
    class Engine
    {
    public:
        void start();
    };
};
```