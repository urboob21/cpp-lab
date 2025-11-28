// cppcheck-suppress-file [functionStatic]

// Prototype is a creational design pattern that lets you copy existing objects
// without making your code dependent on their classes. Appicability:
// (*)  when your code shouldn’t depend on the concrete classes of objects that
// you need to copy.
// (**) when you want to reduce the number of subclasses that only differ in the
// way they initialize their respective objects.

// UML: docs/uml/patterns_behavioral_iterator.drawio.svg

#include <iostream>

namespace {}

// struct SingletonAutoRuner
// {
//     SingletonAutoRuner()
//     {
//         std::cout << "\n--- Singleton Pattern Example ---\n";
//     }
// };

// static SingletonAutoRuner instance;