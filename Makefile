.PHONY: all build cmake clean format 

# the release directory is added so that cmake and cubeide build linker script include statement references the same path (i.e. they have to be the same steps away from submodules folder)
BUILD_DIR := build/release

all: build

${BUILD_DIR}/Makefile:
	cmake \
		-B${BUILD_DIR} \
		-DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-G "Unix Makefiles"

cmake: ${BUILD_DIR}/Makefile

build: cmake
	$(MAKE) -C ${BUILD_DIR} --no-print-directory

####################################3
####################################3

SRCS := $(shell find . -name '*.[ch]' -or -name '*.[ch]pp')
format: $(addsuffix .format,${SRCS})

%.format: %
	clang-format -i $<

clean:
	rm -rf $(BUILD_DIR) $(BUILD_TEST_DIR)
