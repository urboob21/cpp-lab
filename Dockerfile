# Define a new image from Ubuntu 2404
# Dockerfile → defines your environment (compiler, cmake, libraries, tools).

FROM ubuntu:24.04

# Install prerequisites
#  - the backslash \ is used for line continuation
#  - `-y` to automatically confirm installation
RUN \
    # updates the package lists for upgrades for packages that need upgrading,
    apt-get update && \     
    # install cmake, gcc, g++
    apt-get install -y cmake && \  
    apt-get install -y gcc g++  && \
    # install cppcheck
    apt-get install -y cppcheck

# Set the working directory inside the Docker image
WORKDIR /cpp-lab

# Copy all things from the build context (the directory where we run docker build) into the docker image being built
# Copy source code from host (build context) to image because the WORKDIR already set
# COPY . . 
COPY . /cpp-lab

# # Build the main and test targets
# RUN \
#     # create a build folder
#     rm -rf build && mkdir build && \
#     cd build && \
#     cmake .. && \
#     cmake --build .