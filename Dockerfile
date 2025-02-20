FROM balenalib/rpi-raspbian:latest

# Install necessary packages
RUN apt-get update && apt-get install -y \
    cmake \
    make \
    g++ \
    clang-tidy \
    && rm -rf /var/lib/apt/lists/*

# Copy the entire repository into the container
COPY . /app

# Set the working directory
WORKDIR /app

# # Run the build script
# CMD ["./build.sh"]
