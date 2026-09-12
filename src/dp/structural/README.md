> Examples: `dp/structural/{Adapter, Bridge, Composite, Decorator, Facade, Flyweight, Proxy}` - run
> one with `./build/bin/cpp_lab_project --run dp/structural/Decorator`

# Structural Design Patterns

Structural design patterns explain how to assemble objects and classes into larger structures, while keeping these structures flexible and efficient.

---

## 1. Adapter
![Diagram](../../../docs/uml/dp/structural_adapter.drawio.svg)
**Adapter**  
Allows objects with incompatible interfaces to collaborate.

**Real-time example:**  
A travel power adapter. You plug a device from one country into a socket in another country using an adapter to make it compatible.

---

## 2. Bridge
![Diagram](../../../docs/uml/dp/structural_bridge.drawio.svg)
**Bridge**  
Lets you split a large class or a set of closely related classes into two separate hierarchies—abstraction and implementation—which can be developed independently.

**Real-time example:**  
A remote control system. The remote (abstraction) can control different devices (TV, AC, music system) independently without changing the remote design.

---

## 3. Composite
![Diagram](../../../docs/uml/dp/structural_composite.drawio.svg)
**Composite**  
Lets you compose objects into tree structures and then work with these structures as if they were individual objects.

**Real-time example:**  
A company organizational chart. Managers and employees are treated uniformly. A manager can have subordinates, who can also be managers or employees.

---

## 4. Decorator
![Diagram](../../../docs/uml/dp/structural_decorator.drawio.svg)
**Decorator**  
Lets you attach new behaviors to objects by placing these objects inside special wrapper objects that contain the behaviors.

**Real-time example:**  
Adding extra features to a coffee order. You start with a plain coffee and decorate it with milk, sugar, or flavors without changing the original coffee class.

---

## 5. Facade
![Diagram](../../../docs/uml/dp/structural_facade.drawio.svg)
**Facade**  
Provides a simplified interface to a library, a framework, or any other complex set of classes.

**Real-time example:**  
A home automation app. Instead of controlling lights, thermostat, and security system separately, the app provides a single interface to manage everything easily.

---

## 6. Flyweight
![Diagram](../../../docs/uml/dp/structural_flyweight.drawio.svg)
**Flyweight**  
Lets you fit more objects into available memory by sharing common parts of state between multiple objects.

**Real-time example:**  
In a word processor, the same font style and size is shared across multiple characters instead of storing them individually for every character.

---

## 7. Proxy
![Diagram](../../../docs/uml/dp/structural_proxy.drawio.svg)
**Proxy**  
Lets you provide a substitute or placeholder for another object. A proxy controls access to the original object, allowing you to perform something before or after the request reaches the original object.

**Real-time example:**  
A payment gateway proxy. Instead of paying directly to the bank, you use a payment service that validates, logs, and then forwards your request to the bank.
