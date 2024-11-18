
# DMD_Project

**DMD_Project** This project applys mesh operations on ideal and defect meshes to get the part which needs to be filled. It uses the open source library [MeshLib](https://github.com/MeshInspector/MeshLib).

## Table of Contents

- [Prerequisites](#prerequisites)
- [Building](#building)
- [Running](#running)
- [Results](#results)
- [Contributing](#contributing)

## Prerequisites

1. **MeshLib Installation**: Download and install the latest version of MeshLib from [MeshLib Releases](https://github.com/MeshInspector/MeshLib/releases) based on your operating system.

2. **GCC Version Check**: Ensure you have GCC version 10 installed. Check your current version with:
   ```bash
   g++ --version
   ```
   If GCC 10 is not installed, you can install it using:
   ```bash
   sudo apt install g++-10
   ```

## Building

To build the project, follow these steps:

```bash
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=g++-10 ..
make -j8
```

## Running

### Main Boolean Pipeline
Execute the (new, modular and scalable) main boolean pipeline for ideal and defect meshes:
```bash
./meshlib_main
```

Execute the (old) main boolean pipeline for ideal and defect meshes:
```bash
./meshlib_boolean_pipeline
```

### Simple Boolean
Run a simple boolean operation on ideal and defect meshes:
```bash
./meshlib_simple_boolean
```

### Global + Local ICP
Perform Global and Local ICP on ideal and defect meshes:
```bash
./meshlib_global_local_icp
```

## Results

The resulting STL files will be saved in the `meshes` folder.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request. For major changes, please open an issue first to discuss what you would like to change.
