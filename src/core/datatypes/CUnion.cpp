#include <iostream>
#include <string>

// --- Union Definition ---
// All members share the same memory
union UnionDataType
{
    int intValue;
    float floatValue;
    char charValue;

    void printAll() const
    {
        std::cout << "intValue = " << intValue
                  << ", floatValue = " << floatValue
                  << ", charValue = " << charValue << "\n";
    }
};

// --- Demonstrate unique property ---
void unionDemo()
{
    std::cout << "\n--- Union Unique Behavior ---\n";

    UnionDataType u;

    u.intValue = 65;
    std::cout << "After assigning intValue = 65:\n";
    u.printAll(); // Only intValue is meaningful; others show overwritten memory

    u.floatValue = 3.14f;
    std::cout << "After assigning floatValue = 3.14:\n";
    u.printAll(); // Writing floatValue overwrites intValue

    u.charValue = 'A';
    std::cout << "After assigning charValue = 'A':\n";
    u.printAll(); // Writing charValue overwrites both intValue and floatValue

    std::cout << "Size of union = " << sizeof(UnionDataType) << " bytes\n";
    std::cout << "Notice: Only one value is valid at a time.\n";
}

// --- Auto-run struct ---
struct CUnion
{
    CUnion()
    {
        std::cout << "\nCompound type: Union\n";
        unionDemo();
    }
};

// Constructed before main()
static CUnion autoRunUnionInstance;