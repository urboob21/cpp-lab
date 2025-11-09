#include <iostream>
#include <cstdint> // for uint8_t

using namespace std;
// A type defined in terms of other types

// *0. Define some enums
// Unscope enum
enum BasicEnum
{
    enumratorA,
    enumratorB,
    enumratorC
};

// Scoped enum - enumrators are inside enum's scope
enum class ScopeEnumClass
{
    enumratorA,
    enumratorB,
    enumratorC
};

// Scoped enum inside a namespace
namespace EnumNameSpace
{
    enum class ScopeEnumClass
    {
        enumratorA,
        enumratorB,
        enumratorC
    };
}

//  Scoped enum with explicit base type
enum class ScopeEnumClassB : uint8_t
{
    enumratorA = 0,
    enumratorB = 1,
    enumratorC = 2
};

void enums()
{
    cout << "\n--- Enum Type Examples ---\n";
    // *1. Using unscope enum
    [[maybe_unused]] BasicEnum unscope_e = enumratorA;

    // *2. Using scoped enum
    [[maybe_unused]] ScopeEnumClass scope_e_c = ScopeEnumClass::enumratorA;

    // *3. Using scoped enum namespace
    [[maybe_unused]] EnumNameSpace::ScopeEnumClass scope_e_c_n = EnumNameSpace::ScopeEnumClass::enumratorA;

    // *4. Using scoped enum with base type
    [[maybe_unused]] ScopeEnumClassB st = ScopeEnumClassB::enumratorA;
}

// A struct that runs code when its object is created
struct CEnum
{
    CEnum()
    {
        cout << "\n"
             << "\n"
             << "Compound type: Enum\n";

        enums();
    }
};

// All global and static objects are constructed before main() begins.
static CEnum autoRunInstance;
