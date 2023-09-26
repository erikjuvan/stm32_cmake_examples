.PHONY: error debug release build cmake clean format 

# the release directory is added so that cmake and cubeide build linker script include statement references the same path (i.e. they have to be the same steps away from submodules folder)

error:
	@printf "Specify target: debug, release, clean\n"
	@exit 2

debug: BUILD_DIR := build/Debug
debug: BUILD_TYPE := Debug
debug: build

release: BUILD_DIR := build/Release
release: BUILD_TYPE := Release
release: build

build: cmake
	$(MAKE) -C ${BUILD_DIR} --no-print-directory

cmake: ${BUILD_DIR}/Makefile

${BUILD_DIR}/Makefile:
	cmake \
		-B${BUILD_DIR} \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-G "Unix Makefiles"

####################################
####################################

SRCS := $(shell find . -name '*.[ch]' -or -name '*.[ch]pp')
format: $(addsuffix .format,${SRCS})

%.format: %
	clang-format -i $<

clean:
	rm -rf build
