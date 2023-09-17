FROM ubuntu:18.04

# Install necessary tools
RUN apt-get update && \
    apt-get install -y g++-7 cmake make valgrind && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 100 --slave /usr/bin/g++ g++ /usr/bin/g++-7

# Set GCC 7.2.0 as default
RUN update-alternatives --set gcc "/usr/bin/gcc-7"

# Set working directory
WORKDIR /app

# Copy the source files into the container
COPY . .

# Build the project
RUN cmake . && make