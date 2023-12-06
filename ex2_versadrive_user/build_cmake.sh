#!/bin/bash
rm build -rf

# TODO we can't really build two projects at once from the same source,
# since pre_build.sh changes the build_info_generated.h so there would
# incorrect data for other builds

build_configurations=(GMCC_75 GMCC_75_491 GMCC_66 GMCC_66_491 RECHI_81 RECHI_81_491)

time (

for build_conf in "${build_configurations[@]}" ; do

    # Configure
    cmake -B build/$build_conf -DCMAKE_BUILD_TYPE=$build_conf --toolchain=gcc-arm-none-eabi.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -G Ninja

    # Check if configuration failed
    if [ $? -ne 0 ]; then
        exit 1
    fi

    # Build
    cmake --build build/$build_conf -j8 --clean-first

    # Check if build failed
    if [ $? -ne 0 ]; then
        exit 1
    fi

done

)
