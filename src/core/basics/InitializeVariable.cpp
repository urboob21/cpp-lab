#include <iostream>
using namespace std;

void initialize_variable();

// A struct that runs code when its object is created
struct InitializeVariable
{
    InitializeVariable()
    {
        cout << "\n"
             << "\n"
             << "InitializeVariable\n";
        initialize_variable();
    }
};

// All global and static objects are constructed before main() begins.
static InitializeVariable autoRunInstance;

struct Foo
{
    Foo()
    {
        cout << "Default constructor/ default init\n";
    }

    // explicit Foo(int)
    Foo(int)
    {
        cout << "Constructor called with int / copy init\n";
    }

    Foo(const Foo &other)
    {
        std::cout << "Copy constructor called\n";
    }
};

void initialize_variable()
{
    cout << "\n--- Variable Initialization Examples ---\n";
    // There are there common ways to intialize a variable
    // * Default
    int initDefaultVar;
    Foo initDefaultObj;

    // *Traditional
    //      * copy-init: Type var = value;
    // 1. Compiler tries to create a temporary Foo from 2.3 by implicit conversion (calls Foo(<implicit int to double>) ).
    // 2. If constructor is explicit, implicit conversion is not allowed → error.
    // 3. Otherwise, temporary Foo is created and then copy/move into copyInitObj2.
    Foo copyInitObj = 2.3; // implicit 2.3(float) -> 2(int) -> call Foo(int) -> create a temporary Foo -> is copied or moved into copyInitObj

    //      * direct-init: Type var(value);
    Foo directInitObj(4);    // call Foo(int)
    Foo directInitObj2(4.3); // look for constructor -> implicit 4.3(float) -> 4(int) -> call Foo(int) ->

    //      * Brace init
    // calls the constructor directly without allowing implicit conversions.
    Foo braceInit{3};
    // Foo braceInit2{3.3}; // ERORR => Prefer this way
}