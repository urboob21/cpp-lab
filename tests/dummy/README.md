# dummy

Scratch files that are **not** part of the build or the test suite (the test
target only compiles `*Test.cpp`).

- `UsbHostSerial_read.cpp` - a standalone reproduction written for a pull
  request in https://github.com/bertmelis/USBHostSerial. It has no `main()`,
  so it only compiles to an object file:

  ```bash
  g++ -std=c++20 -c tests/dummy/UsbHostSerial_read.cpp -o /tmp/usb_read.o
  ```

  To run `testRead()`, add a `main()` that calls it.
