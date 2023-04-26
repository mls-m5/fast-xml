# Use the official Ubuntu image as the base image
FROM ubuntu:latest

# Update packages and install necessary dependencies
RUN apt-get update

RUN apt-get install -y g++ git wget

# Install a newer version of CMake
RUN wget -qO- "https://github.com/Kitware/CMake/releases/download/v3.23.0/cmake-3.23.0-linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C /usr/local

# Change to the project directory
WORKDIR /fast-xml

# Run the tests (the command will be overridden by docker-compose)
CMD echo "Use docker-compose to build and test the project"
