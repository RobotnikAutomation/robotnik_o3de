# Robotnik O3DE - Simulation

> **⚠️ Work in Progress**  
> This repository is currently under development and is intended for evaluation and benchmarking purposes. The primary goal is to assess O3DE's simulation capabilities and compare them with other simulation environments like Gazebo.

## 📋 Overview

`robotnik_o3de` leverages the Open 3D Engine (O3DE) to create realistic simulations of Robotnik robots. This package provides tools and resources to simulate robot behavior, sensor data, and interactions within a virtual environment for benchmarking and comparative analysis.

## O3DE Installation Guide with ROS 2 Gems

Installation guide for O3DE with ROS 2 gems for the benchmark project.

### Simulator Installation

Download the O3DE `.deb` package from:

https://o3debinaries.org/download/linux.html

Install it with:

```bash
sudo apt install <path_to_deb_package>/<debian_package_name>.deb
```

Download the extra gems:

```bash
git clone https://github.com/o3de/o3de-extras
```

Set environment variables:

```bash
export O3DE_HOME=/opt/O3DE/26.05/

export O3DE_EXTRAS_HOME=${HOME}/o3de-extras
```

Download the gem assets:

```bash
sudo apt install git-lfs

cd ${O3DE_EXTRAS_HOME}

git lfs install && git lfs pull
```

Register the downloaded gems:

```bash
${O3DE_HOME}/scripts/o3de.sh register --all-gems-path ${O3DE_EXTRAS_HOME}/Gems/

${O3DE_HOME}/scripts/o3de.sh register --all-templates-path ${O3DE_EXTRAS_HOME}/Templates/
```

### Project Build

Source ROS 2 and install dependencies:

```bash
source /opt/ros/jazzy/setup.bash

sudo apt update

sudo apt install ros-${ROS_DISTRO}-ackermann-msgs ros-${ROS_DISTRO}-control-msgs ros-${ROS_DISTRO}-nav-msgs ros-${ROS_DISTRO}-gazebo-msgs ros-${ROS_DISTRO}-xacro ros-${ROS_DISTRO}-vision-msgs
```

Set the project variables you want to build:

```bash
export PROJECT_NAME=robotnik_roscon25

export PROJECT_PATH=${HOME}/projects/robotnik_o3de/project/${PROJECT_NAME}
```

Clone and register the project:

```bash
cd ${HOME}/projects

git clone -b humble-devel https://github.com/RobotnikAutomation/robotnik_o3de.git

cd ${PROJECT_PATH}

${O3DE_HOME}/scripts/o3de.sh register --project-path ${PROJECT_PATH}
```

Download project assets:

```bash
cd ${PROJECT_PATH}

git lfs install && git lfs pull
```

Configure and build:

```bash
cd ${PROJECT_PATH}

cmake -B build/linux -G "Ninja Multi-Config" -DLY_DISABLE_TEST_MODULES=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DLY_STRIP_DEBUG_SYMBOLS=ON

cmake --build build/linux --config profile --target ${PROJECT_NAME} Editor ${PROJECT_NAME}.Assets ${PROJECT_NAME}.GameLauncher
```

### Project Execution

There are different ways to launch it.

#### From the O3DE command:

```bash
cd ${PROJECT_PATH}

./build/linux/bin/profile/robotnik_roscon25.GameLauncher
```

#### From Robotnik launchers:

```bash
cd ${HOME}/projects/robotnik_o3de/robotnik_o3de

colcon build

source install/setup.bash
```

Terminal 1:

```bash
ros2 launch robotnik_o3de spawn_world.launch.py
```

Terminal 2:

```bash
ros2 launch robotnik_o3de spawn_robot.launch.py robot_id:=robot_a robot:=rbwatcher
```

#### From Benchmark:

```bash
cd ${HOME}/benchmark_ws

python3 ./scripts/benchmark_simulator.py --category 1 --iterations 1 --iteration_time 60 o3de
```

### Installation Issues Found

- Installation instructions were not very clear: there are several build command variations.
- The `git lfs` step was initially skipped because it was not installed via `apt`; during build, many prefabs failed due to missing assets. After running the LFS commands, the robot appeared correctly in simulation.
- There was a DDS issue with Fast DDS that was fixed by switching to Cyclone DDS. The error appeared when launching the `spawn_robot` service, falsely indicating another service with the same name was already running.
