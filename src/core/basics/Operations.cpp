#include <iostream>
using namespace std;

void arithmeticOperator();
void logicalOperator();
void bitWiseOperator();

struct Operations
{
    Operations()
    {
        cout << "\n"
             << "\n"
             << "Operation\n";
        arithmeticOperator();
        logicalOperator();
        bitWiseOperator();
    }
};

static Operations autoRunInstance;

void arithmeticOperator()
{
    cout << "\n--- ArithmeticOperator Examples ---\n";
    int a{100}, b{200};

    // Addition
    cout << "a = " << a << ", b = " << b << "\n";
    int sum = a + b;
    cout << "sum = " << sum << "\n";

    // Subtraction
    cout << "a = " << a << ", b = " << b << "\n";
    int different = a - b;
    cout << "different = " << different << "\n";

    // Multiplication
    cout << "a = " << a << ", b = " << b << "\n";
    int product = a * b;
    cout << "product = " << product << "\n";

    // Division
    cout << "a = " << a << ", b = " << b << "\n";
    int quotient = a / b;
    cout << "quotient = " << quotient << "\n";

    // Modulus
    cout << "a = " << a << ", b = " << b << "\n";
    int remainder = a % b;
    cout << "remainder = " << remainder << "\n";

    // Increment
    cout << "a = " << a << "\n";
    int preIn = ++a; // increase a, return copy
    cout << "preIn = " << preIn << "\n";

    cout << "a = " << a << "\n";
    int postIn = a++; // copy a to a copy, then increase a, return copy
    cout << "postIn = " << postIn << "\n";

    // Decrement
    cout << "b = " << b << "\n";
    int preDe = --b;
    cout << "preDe = " << preDe << "\n";

    cout << "b = " << b << "\n";
    int postDe = b--;
    cout << "postDe = " << postDe << "\n";

    // Comma:
    int value = (a++, b); // a is incremented, then b is returned
    cout << "a = " << a << ", b = " << b << "\n";
    cout << "comma(a++, b) = " << value << "\n";
}

void logicalOperator()
{
    cout << "\n--- LogicalOperator Examples ---\n";
    bool a = true;
    bool b = false;
    bool c = true;

    cout << boolalpha; // show true/false instead of 1/0
    cout << "a = " << a << ", b = " << b << ", c = " << c << "\n\n";

    // AND (&&)
    cout << "[AND] a && b = " << (a && b) << "\n";

    // OR (||)
    cout << "[OR ] a || b = " << (a || b) << "\n";

    // NOT (!)
    cout << "[NOT] !c = " << (!c) << "\n";
}

#include <bitset>
void bitWiseOperator()
{
    cout << "\n--- BitWiseOperator Examples ---\n";
    bitset<8> bitsA{0b1111'1111};
    bitset<8> bitsB{0b1111'0000};

    cout << "bitA = " << bitsA << ", bitB = " << bitsB << "\n";

    // AND
    bitset<8> result = bitsA & bitsB;
    cout << "bitA && bitB= " << result << "\n";

    // OR
    result = bitsA | bitsB;
    cout << "bitA | bitB= " << result << "\n";

    // XOR
    result = bitsA ^ bitsB;
    cout << "bitA ^ bitB= " << result << "\n";

    // NOT
    result = ~bitsA;
    cout << "~bitA = " << result << "\n";

    // LEFT SHIFT
    result = bitsA << 1;
    cout << "bitA << 1 = " << result << "\n";

    // RIGHT SHIFT
    result = bitsA >> 1;
    cout << "bitA >> 1 = " << result << "\n";
}