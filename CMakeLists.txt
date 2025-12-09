cmake_minimum_required(VERSION 3.10)
project(ProcessDemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include common directory
include_directories(${CMAKE_SOURCE_DIR}/common)

# Common IPC source files
set(IPC_SOURCES
    common/ipc/SharedMemory.cpp
    common/ipc/SharedMemory_Producer.cpp
    common/ipc/SharedMemory_Consumer.cpp
)

# Process 1 executable
add_executable(process1 process1/main.cpp ${IPC_SOURCES})
target_include_directories(process1 PRIVATE ${CMAKE_SOURCE_DIR}/common)
target_include_directories(process1 PRIVATE ${CMAKE_SOURCE_DIR}/common/ipc)

# Process 2 executable
add_executable(process2 process2/main.cpp ${IPC_SOURCES})
target_include_directories(process2 PRIVATE ${CMAKE_SOURCE_DIR}/common)
target_include_directories(process2 PRIVATE ${CMAKE_SOURCE_DIR}/common/ipc)

# Test executable - IPC Queue Connection and Crash Recovery Test
add_executable(test_ipc_crash tests/test_ipc_crash.cpp ${IPC_SOURCES})
target_include_directories(test_ipc_crash PRIVATE ${CMAKE_SOURCE_DIR}/common)
target_include_directories(test_ipc_crash PRIVATE ${CMAKE_SOURCE_DIR}/common/ipc)