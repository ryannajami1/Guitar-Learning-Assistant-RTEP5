#!/bin/sh

# Store the original working directory
original_dir=$(pwd)

# Get the directory where the script is located
script_dir=$(dirname "$0")

# Change to the script's directory
echo "Changing to $script_dir"
cd "$script_dir"

# Initialize CMake arguments
cmake_args="-G 'Unix Makefiles' -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"

# Check for --debug argument
if [ "$1" = "--debug" ]; then
    cmake_args="$cmake_args -DCMAKE_BUILD_TYPE=Debug"
    echo "Making debug build"
    shift
fi

# Example with multiple arguments:
if [ $# -ge 1 ]; then
    echo "The argument provided for cmake was: $1"
    cmake_args="$cmake_args $1"
fi

# Create build directory if it doesn't exist, -p flag prevents error if it already exists
mkdir -p build

# Change to the build directory
cd build

# build the project with the specified arguments
eval cmake $cmake_args ..
make

# Change back to the original directory
cd "$original_dir"

echo "Build process completed. Returning to $original_dir"
