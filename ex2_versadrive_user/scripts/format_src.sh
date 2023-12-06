#!/bin/bash
find ../src/app/ -type f | egrep '\.(cpp|hpp|c|h|cc|hh)$' | xargs clang-format -i -style=file