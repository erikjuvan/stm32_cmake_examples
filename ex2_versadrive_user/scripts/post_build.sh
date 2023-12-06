#!/bin/bash

build_file_base_name=$1

# Print commands as they are executed
set -x

# Add version to hex filename
sh ../../scripts/add_version.sh $build_file_base_name

# Remove original hex so we only keep the renamed one, to simplify things
rm ${build_file_base_name}.hex