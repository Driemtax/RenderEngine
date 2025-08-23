# RenderEngine

A lightweight CPU-based 3D rendering engine built from scratch in C++ using only SDL2 for window management and pixel manipulation. This is based on the wonderful explanations of [OneLoneCoder](https://github.com/onelonecoder), make sure to check out his stuff on Youtube!

## 🚀 Current Features

- **CPU-only 3D rendering** - No GPU dependencies, all calculations done on CPU
- **Basic 3D pipeline** - Vertex transformation, projection, and rasterization
- **Wireframe and filled triangle rendering**
- **Real-time mesh rotation** - Smooth frame-rate independent animation
- **Perspective projection** - Configurable field of view and clipping planes
- **Single-header engine design** - Easy integration into any project

## 🎯 Work in Progress

This is an active learning project with planned features:

- [ ] **Wavefront OBJ file import** - Load 3D models from Blender and other tools
- [ ] **Basic lighting system** - Ambient, diffuse, and specular lighting
- [ ] **Texture mapping** - Apply 2D textures to 3D surfaces
- [ ] **Camera controls** - Move and rotate the view in 3D space

## 📋 Prerequisites

- **C++11 or later**
- **SDL2 development libraries**
- **CMake 3.10+** (optional, but recommended)

## 🛠️ Building the Project

### Using CMake (Recommended)
```bash
git clone https://github.com/Driemtax/RenderEngine.git
cd RenderEngine/cpp-sdl2-project
mkdir build
cd build
cmake ..
make
./cpp-sdl2-project
```

## 💻 Usage

### Basic Example
view the `main.cpp` to see how to use the engine

## 🏗️ Project Structure

```
RenderEngine/
├── cpp-sdl2-project/
│   └── src/
│       ├── main.cpp                 # Application entry point
│       ├── engine/
│       │   └── engine.h             # Single-header render engine
│       ├── models/
│       │   ├── float3.cpp           # 3D vector structure
│       │   ├── triangle.cpp         # Triangle primitive
│       │   ├── mesh.cpp             # Collection of triangles
│       │   └── int2vec.cpp          # 2D integer vector
│       └── helpers/
│           └── math.h               # Mathematical utility functions
├── build/                           # CMake build directory
├── CMakeLists.txt                   # Build configuration
└── README.md
```

## 🤝 Contributing

This is a learning project, but suggestions and improvements are welcome! Feel free to:
- Report bugs
- Suggest new features
- Share your own modifications

## 🎓 Educational Purpose

This engine is designed as a learning tool to understand:
- 3D mathematics and transformations
- Rasterization algorithms
- Graphics pipeline fundamentals
- Real-time rendering concepts

The focus is on simplicity and educational value
