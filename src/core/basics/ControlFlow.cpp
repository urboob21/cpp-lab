#include <iostream>
#include <random>
using namespace std;

void conditionals()
{
    cout << "\n--- Conditional Examples ---\n";
    // if-else
    int x = rand();
    cout << "x = " << x << "\n";
    if (x > 0)
    {
        cout << "x is positive\n";
    }
    else if (x < 0)
    {
        cout << "x is negative\n";
    }
    else
    {
        cout << "x is zero\n";
    }

    // switch
    int choice = rand() % 2 + 1;
    cout << "choice = " << x << "\n";
    switch (choice)
    {
    case 1:
        cout << "Choice is 1\n";
        break;
    case 2:
        cout << "Choice is 2\n";
        break;
    default:
        cout << "Choice is something else\n";
        break;
    }
}

void jumps()
{
    cout << "\n--- Jump Statement Examples ---\n";

    // goto
    int num = rand();
    if (num == 3)
        goto jumpLabel;
    cout << "This line will be skipped.\n";

jumpLabel:
    cout << "Jumped here using goto!\n";

    // break / continue
    for (int i = 0; i < 5; ++i)
    {
        if (i == 2)
            continue; // skip 2
        if (i == 4)
            break; // stop loop at 4
        cout << "i = " << i << "\n";
    }
}

int square(int n)
{
    // return
    return n * n;
}

void functionCalls()
{
    cout << "\n--- Function Call Examples ---\n";
    // function call
    int result = square(5);
    cout << "square(5) = " << result << "\n";
}

void loops()
{
    cout << "\n--- Loop Examples ---\n";

    // while
    int i = 0;
    while (i < 3)
    {
        cout << "while loop i = " << i << "\n";
        ++i;
    }

    // do-while
    int j = 0;
    do
    {
        cout << "do-while loop j = " << j << "\n";
        ++j;
    } while (j < 2);

    // for
    for (int k = 0; k < 3; ++k)
    {
        cout << "for loop k = " << k << "\n";
    }

    // ranged-for
    const int arr[] = {10, 20, 30};
    for (int value : arr)
    {
        cout << "ranged-for value = " << value << "\n";
    }
}

void halts()
{
    cout << "\n--- Halt Examples ---\n";

    // std::exit() — terminates the program normally
    // std::abort() — terminates abnormally (no cleanup)
    // Uncomment one at a time to see behavior

    // std::exit(0);
    // std::abort();
}

void exceptions()
{
    cout << "\n--- Exception Handling Examples ---\n";

    // try - catch - throw
    try
    {
        throw runtime_error("Something went wrong!");
    }
    catch (const exception &e)
    {
        cout << "Caught exception: " << e.what() << "\n";
    }
}

// A struct that runs code when its object is created
struct ControlFlow
{
    ControlFlow()
    {
        cout << "\n"
             << "\n"
             << "ControlFlow\n";
        conditionals();
        jumps();
        functionCalls();
        loops();
        halts();
        exceptions();
    }
};

// All global and static objects are constructed before main() begins.
static ControlFlow autoRunInstance;
