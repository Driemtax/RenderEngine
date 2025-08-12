# C++ SDL2 Project

This project demonstrates how to create a simple C++ application using the SDL2 library. The application opens a black window and remains open until closed by the user.

## Project Structure

```
cpp-sdl2-project
├── src
│   └── main.cpp        # Entry point of the application
├── include             # Directory for header files (currently empty)
├── Makefile            # Build instructions for the project
├── CMakeLists.txt      # CMake configuration file
└── README.md           # Project documentation
```

## Prerequisites

Make sure you have the SDL2 library installed on your system. You can install it using the following commands:

```bash
sudo apt-get install libsdl2-2.0-0
sudo apt-get install libsdl2-dev
```

## Building the Project

You can build the project using either the Makefile or CMake.

### Using Makefile

1. Open a terminal and navigate to the project directory.
2. Run the following command to build the project:

   ```bash
   make
   ```

### Using CMake

1. Create a build directory:

   ```bash
   mkdir build
   cd build
   ```

2. Run CMake to configure the project:

   ```bash
   cmake ..
   ```

3. Build the project:

   ```bash
   make
   ```

## Running the Application

After building the project, you can run the application with the following command:

```bash
./src/main
```

This will open a black window. You can close it by clicking the close button on the window.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.