#!/bin/sh
set -e

# Store the original working directory
original_dir=$(pwd)

# Get the directory where the script is located
script_dir=$(dirname "$0")

# Change to the script's directory
echo "Changing to $script_dir"
cd "$script_dir"

# Example with multiple arguments:
if [ $# -ge 1 ]; then
    echo "The argument provided for cmake was: $1"
fi

# Create build directory if it doesn't exis, -p flag prevents error if it already exists
mkdir -p build

# Change to the build directory
cd build

# build the project
cmake -G "Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $1 ..
make

# Change back to the original directory
cd "$original_dir"

echo "Build process completed. Returning to $original_dir"
