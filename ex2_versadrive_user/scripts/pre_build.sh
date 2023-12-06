#!/bin/bash

ProjName=$1
ConfigName=$2
HostOsName=$3

# Print commands as they are executed
set -x

# Generate build_info.c
sh ../../scripts/build_info.sh $ProjName $ConfigName "$HostOsName"