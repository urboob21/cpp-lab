// cppcheck-suppress-file [functionStatic]

// Abstract Factory is a creational design pattern that lets you produce
// families of related objects without specifying their concrete classes.
// Appicability:
// (*)  when your code needs to work with various families of related products,
// but you don’t want it to depend on the concrete classes
//      of those products—they might be unknown beforehand or you simply want to
//      allow for future extensibility.
// (**) when you have a class with a set of Factory Methods that blur its
// primary responsibility.

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