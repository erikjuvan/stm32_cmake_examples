#!/bin/bash

# EDITABLE! SW Release Purpose Type ID TODO: can/should we move the number somewhere else?
# Perhaps make it similar to the way SW_MODEL_ID is implemented. Defined in version.c,
# extracted with readelf and parsed to get the value. This way post_build never needs to be edited.
sw_release_purpose_type_id="90" # DEV release: 90, Production release: 00

# First parameter is the name of the configuration
build_file_base_name="$1"

# Get the current date in the desired format (YYYYMMDD)
release_date=$(date "+%Y%m%d")

# Get SW model ID
# Explanation: first byte of section .user_ver contains version_image[4] from version.c.
# First byte in the array is the SW_MODEL_ID macro, which is what we are after here.
sw_model_id_hex=$(arm-none-eabi-readelf -x .user_ver $build_file_base_name.elf | tail -n +2 | awk '{print $2}' | xargs echo | cut -c6-7)
sw_model_id=$(printf "%02d" "0x$sw_model_id_hex")

# Search for version macros in version.c
ver_file="../../src/app/version.c"
major_def=$(grep "#define FIRMWARE_VERSION_MAJOR" "$ver_file" | awk '{print $3}')
minor_def=$(grep "#define FIRMWARE_VERSION_MINOR" "$ver_file" | awk '{print $3}')
patch_def=$(grep "#define FIRMWARE_VERSION_PATCH" "$ver_file" | awk '{print $3}')

# Create new file name
filename="${build_file_base_name}_$release_date"
filename="${filename}_$sw_model_id"
filename="${filename}_$sw_release_purpose_type_id"
filename="${filename}_${major_def}.${minor_def}.${patch_def}"

# Create a copy of the HEX file with the new name
echo "Copy $build_file_base_name.hex to $filename.hex"
cp "$build_file_base_name.hex" "$filename.hex"