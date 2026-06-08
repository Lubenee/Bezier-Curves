# Bezier-Curves

An OpenGL tool to create Bezier curves.

# Bezier Curve

## Features

- **Dynamic Point Dragging**
- **Bezier Transition Points**

## Requirements

- **C++17** or newer
- **OpenGL 4.5+** (Linux) or **OpenGL 4.1+** (macOS)
- **GLFW**, **GLEW**, and **GLM** libraries
- **CMake** (3.10+) and **PkgConfig**

## Setup Instructions

### macOS Setup (Brand New Mac)

To build and run this application on a brand new macOS system, run the following steps in your terminal:

1. **Install Command Line Tools** (compilers and git):
   ```bash
   xcode-select --install
   ```

2. **Install Homebrew** (package manager):
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. **Install build tools and required libraries**:
   ```bash
   brew install cmake pkg-config glfw glew glm
   ```

### Ubuntu Setup

Run the following command to install the necessary packages on Ubuntu:
```bash
sudo apt update && sudo apt install -y build-essential cmake pkg-config libglfw3-dev libglew-dev libglm-dev
```

## How to Build & Run

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Lubenee/Bezier-Curves.git
   cd Bezier-Curves
   ```

2. **Configure and Build using CMake**:
   ```bash
   cmake -B build
   cmake --build build
   ```

3. **Run the executable**:
   ```bash
   ./Bezier_Curves
   ```

## Compatibility

- Works on Ubuntu with GCC 11 / Clang.
- Works on macOS (both Apple Silicon and Intel) with Apple Clang 15+.
