#!/bin/bash

# Generate build info string that will be put into
# separate .build_info section

#pass input argumets to variables
proj_name=$1          # ${ProjName}
config_name=$2        # ${ConfigName}
host_os_name=$3       # '${HostOsName}'

####################################################################
echo "Generating build_info_generated.h"

build_info_file=../../src/app/build_info_generated.h

build_date=$(date)

git_branch=dummy

if [[ $git_branch ]]; then
  :
else
  echo "Failed to determine current git branch"
  exit 1
fi

git_count=12

if [[ $git_count ]]; then
  :
else
  echo "Failed to determine commit count"
  exit 1
fi

git_log="Just some demo text"
gcc_version=$(arm-none-eabi-gcc --version | grep eabi)

# Search for version
ver_file="../../src/app/version.c"
major_def=$(grep "#define FIRMWARE_VERSION_MAJOR" "$ver_file" | awk '{print $3}')
minor_def=$(grep "#define FIRMWARE_VERSION_MINOR" "$ver_file" | awk '{print $3}')
patch_def=$(grep "#define FIRMWARE_VERSION_PATCH" "$ver_file" | awk '{print $3}')
proj_version=$major_def.$minor_def.$patch_def

# Echo build info to stdout
echo "Proj Name  : $proj_name"
echo "Version    : $proj_version"
echo "Config     : $config_name"
echo "Git Branch : $git_branch$dirty"
echo "Commit cnt : $git_count"
echo "Git Commit : $git_log"
echo "Build Date : $build_date"
echo "Compiler   : $gcc_version"
echo "Built On   : $host_os_name"

####################################################################
echo ""                                                                                 >  $build_info_file

# values of script arguments
echo "// Script arguments:"                                                             >> $build_info_file
echo "// \$1 = proj_name     = $1"                                                      >> $build_info_file
echo "// \$2 = config_name   = $2"                                                      >> $build_info_file
echo "// \$3 = host_os_name  = $3"                                                      >> $build_info_file
echo ""                                                                                 >> $build_info_file
echo "// Build info file path relative to build folder"                                 >> $build_info_file
echo "// build_info_file = $build_info_file"                                            >> $build_info_file
echo ""                                                                                 >> $build_info_file
echo ""                                                                                 >> $build_info_file

# build info
echo "////////////////////////////////////////////////////////////////////////////////" >> $build_info_file
echo "// Constants"                                                                     >> $build_info_file
echo "////////////////////////////////////////////////////////////////////////////////" >> $build_info_file
echo ""                                                                                 >> $build_info_file
echo "/**"                                                                              >> $build_info_file
echo " * Build info string."                                                            >> $build_info_file
echo " */"                                                                              >> $build_info_file
echo "#define BUILD_INFO_GENERATED \\"                                                  >> $build_info_file
echo "\"-- Project build info --\n\"\\"                                                 >> $build_info_file
echo "\"Proj Name  : $proj_name\n\"\\"                                                  >> $build_info_file
echo "\"Version    : $proj_version\n\"\\"                                               >> $build_info_file
echo "\"Config     : $config_name\n\"\\"                                                >> $build_info_file
echo "\"Git Branch : $git_source_info$dirty\n\"\\"                                      >> $build_info_file
echo "\"Git Commit : $git_log\n\"\\"                                                    >> $build_info_file
echo "\"Build Date : $build_date\n\"\\"                                                 >> $build_info_file
echo "\"Compiler   : $gcc_version\n\"\\"                                                >> $build_info_file
echo "\"Built On   : $host_os_name\n\""                                                 >> $build_info_file
echo ""                                                                                 >> $build_info_file
echo ""                                                                                 >> $build_info_file
echo "////////////////////////////////////////////////////////////////////////////////" >> $build_info_file
echo "/**"                                                                              >> $build_info_file
echo "* @} <!-- END GROUP -->"                                                          >> $build_info_file
echo "*/"                                                                               >> $build_info_file
echo "////////////////////////////////////////////////////////////////////////////////" >> $build_info_file