#!/bin/bash
rm build -rf

(time {
cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug --toolchain=gcc-arm-none-eabi.cmake -G Ninja
cmake --build build/debug -j8 --clean-first
}) &

(time {
cmake -B build/release -DCMAKE_BUILD_TYPE=Release --toolchain=gcc-arm-none-eabi.cmake -G Ninja
cmake --build build/release -j8 --clean-first
}) &

wait
