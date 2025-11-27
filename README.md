## 1. OVERVIEW
**Project Structure**
```
includes/   → Header files (.h, .hpp)  
src/        → Source files (.cpp)  
tests/      → GoogleTest test cases  
```
## 2. DEPENDENCIES
Make sure the following tools are installed before building the project:
- **g++ / gcc**
- **CMake**
- **Git**
- **lcov** (for code coverage)
- **cppcheck** (for static analysis)
- **Optional:** Install `clang-format` to format C++ code.

Linux
```bash
sudo apt install clang-format
find . -regex '.*\.\(cpp\|h\|hpp\)' -exec clang-format -i {} \;
```
Windows
```bash
# Install clang-format via Chocolatey
choco install clang-format

# Apply clang-format recursively to .cpp, .h, .hpp files
Get-ChildItem -Recurse -Include *.cpp, *.h, *.hpp | ForEach-Object { clang-format -i $_.FullName }
```
## 3. SETUP
* Setup the Local Test Environment
    * 1.Using your own Ubuntu system
        * Install `gcc`, `cmake`, `git`, and `pthread` (Skip this step if you already install)
            ```
            $ sudo apt-get update
            $ sudo apt-get install g++
            $ sudo apt-get install lcov
            $ sudo apt-get install cmake
            $ sudo apt-get install git
            $ sudo apt-get install cppcheck
            ```
        * Build the application and the tests
            ```
            $ cd build
            $ cmake ..
            $ cmake --build .
            ```
        * Run the application and the test
            ```
            $ ./cpp_lab_project
            $ ./cpp_lab_project_test
            ```
        * (Optional) Run static analysis
            ```
            $ sudo apt-get install cppcheck
            $ cppcheck "folder" / "file"
            ```
    * 2.Using **Docker**
        * Build the Docker image
            ```
            docker build --tag sample-ci-cpp .
            ```
        * Run an interactive container
            ```
            docker run -it sample-ci-cpp:latest /bin/bash
            ```
        * Inspect the environment
            ```
            printenv
            ```
        * *Notes:*
            * Use the -t or --tag flag to set the name of the image to be created. (the full name is actually sample-ci-cpp:latest, since latest is the default tag)
            * Opening an interactive shell inside your Docker container to explore, test, or debug the environment built from your image.
            * docker run to start a new container.
            * -it → run it interactively:
            * -i = keep STDIN open (so you can type commands)
            * -t = allocate a terminal (TTY)
            * sample-ci-cpp:latest → the image you built earlier.
            * /bin/bash → the command to execute inside the container (opens a Bash shell).

## 4. DOCUMENTATIONS