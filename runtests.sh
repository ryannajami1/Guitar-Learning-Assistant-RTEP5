#!/bin/sh
set -e

# Store the original working directory
original_dir=$(pwd)

# Get the directory where the script is located
script_dir=$(dirname "$0")

# Change to the script's directory
echo "Changing to $script_dir"
cd "$script_dir"

# Change to the build directory
cd build

# build the project
ctest

# Change back to the original directory
cd "$original_dir"

echo "Build process completed. Returning to $original_dir"
