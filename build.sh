#!/bin/bash

# Define the build directory and executable directory
BUILD_DIR="data/build"
EXEC_DIR="data/bin"  # Ensure this matches the actual location of the executable

# Remove previous build directory and recreate it
echo "Cleaning old build..."
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR

# Navigate into the build directory
cd $BUILD_DIR

# Run CMake to configure the project
echo "Configuring project..."
cmake ../..

# Build the project
echo "Building project..."
make -j$(nproc) # Using nproc to utilize all available processors

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful"
    # Run the executable if the build was successful
    
else
    echo "Build failed"
    exit 1
fi

