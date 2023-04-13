# Instrumentation library 
## Install kernel module
Follow steps from https://github.com/trailofbits/tsc_freq_khz#building-and-usage
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
set(MYLIBRARY_DIR /home/sujit/projects/instrumentation)
include_directories(${MYLIBRARY_DIR}/include)
link_directories(${MYLIBRARY_DIR}/build)

add_executable(myapp main.cpp)
target_include_directories(myapp PRIVATE instrumentation)
```
