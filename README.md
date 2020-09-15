## NMS

This extremely small library contains functionality to do non-maximum suppression on set of bounding boxes.  
No external dependencies for usage is required.

## Requirements
* conan

#### Includes
* gtest
* benchmark
* opencv

Note: NMS itself has no external dependencies except gtest. For demo it uses conan to install gtest, benchmark, opencv.

## How to build

Project was tested on ubuntu 18 with GCC 7 compiler.

In the project root:

`$ mkdir build`  
`$ cd build`  
`$ conan install ..`  
`$ cmake  -DCMAKE_BUILD_TYPE=Release ..`  
`$ make -j6`  

