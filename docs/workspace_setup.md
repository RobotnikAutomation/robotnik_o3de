# Workspace Setup for Robotnik O3DE Simulation

This document provides step-by-step instructions to set up the workspace for the Robotnik O3DE simulation environment.

## Prerequisites

```bash
export O3DE_HOME=${HOME}/o3de
export O3DE_EXTRAS_HOME=${HOME}/o3de-extras

export PROJECT_NAME=robotnik_roscon25
export PROJECT_PATH=${HOME}/projects/${PROJECT_NAME}
```

## Build environment

```bash
cd $PROJECT_PATH
# Create build directory and configure the project
cmake -B build/linux -G "Ninja Multi-Config" -DLY_DISABLE_TEST_MODULES=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DLY_STRIP_DEBUG_SYMBOLS=ON
cmake --build build/linux --config profile --target ${PROJECT_NAME} Editor ${PROJECT_NAME}.Assets
```

## Run Simulation Editor

Editor is used to create and modify levels, assets, and configurations for the simulation.

```bash
cd $PROJECT_PATH
# Run Editor
./build/linux/bin/profile/Editor
```

## Run Simulation Release

To run the simulation in release mode, use the following commands:

```bash
cd $PROJECT_PATH
# Build the project for release
cmake --build build/linux --config profile --target ${PROJECT_NAME} ${PROJECT_NAME}.Assets ${PROJECT_NAME}.GameLauncher
# Run the simulation
./build/linux/bin/profile/robotnik_roscon25.GameLauncher
```
