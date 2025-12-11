# Prototyping-IPC




mkdir build && cd build

cmake ..

cmake --build .

./process1 9001 & ./process2 9002 &