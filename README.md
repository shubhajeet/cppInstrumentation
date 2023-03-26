# Instrumentation library 

## Build
```bash
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release .. & make -j
```

## Test
```bash
cd build
./build/test_library
```
## Using library
```
add_executable(myapp main.cpp)
target_include_directories(myapp PRIVATE /path/to/mylibrary)
```