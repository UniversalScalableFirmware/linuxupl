
export WORKSPACE=$(dir $(abspath $(firstword $(MAKEFILE_LIST))))

IMAGE_ENTRY_POINT = _ModuleEntryPoint

#
# Build Directory Macro Definition
#
MODULE_BUILD_DIR = $(WORKSPACE)../Build
OUTPUT_DIR = $(WORKSPACE)../Build/OUTPUT
DEBUG_DIR = $(WORKSPACE)../Build/DEBUG
MAKE_FILE = $(WORKSPACE)GNUmakefile

#
# Build Macro
#

OBJECT_FILES =  \
    $(OUTPUT_DIR)/QuickSort.o \
    $(OUTPUT_DIR)/LinuxUniversalPayloadEntry.o \
    $(OUTPUT_DIR)/MemoryMapLib.o \
    $(OUTPUT_DIR)/InternalSwitchStack.o \
    $(OUTPUT_DIR)/LoadLinux.o \
    $(OUTPUT_DIR)/Hob.o

STATIC_LIBRARY_FILES =  \
    $(OUTPUT_DIR)/LinuxUniversalPayloadEntry.lib

#
# Overridable Target Macro Definitions
#

CODA_TARGET = $(WORKSPACE)../Build/DEBUG/LinuxUniversalPayloadEntry.elf \

#
# Default target, which will build dependent libraries in addition to source files
#

all: mbuild

#
# Target used when called from platform makefile, which will bypass the build of dependent libraries
#

pbuild: init $(CODA_TARGET)

#
# ModuleTarget
#

mbuild: init gen_libs $(CODA_TARGET)


#
# Build Target used in multi-thread build mode, which will bypass the init and gen_libs targets
#

tbuild:  $(CODA_TARGET)


#
# Target to update the FD
#

fds: mbuild gen_fds

#
# Initialization target: print build information and create necessary directories
#
init: info dirs

info:
	-@echo Building ...
	-@echo $(WORKSPACE)
	-@echo $(MAKE_FILE)

dirs:
	-@mkdir -p $(DEBUG_DIR)
	-@mkdir -p $(OUTPUT_DIR)

#
# GenLibsTarget
#
gen_libs:
	@cd $(MODULE_BUILD_DIR)

#
# Build Flash Device Image
#
gen_fds:
	@make -f $(WORKSPACE)GNUmakefile fds


#
# Individual Object Build Targets
#

$(OUTPUT_DIR)/MemoryMapLib.o : $(MAKE_FILE)
$(OUTPUT_DIR)/MemoryMapLib.o : $(WORKSPACE)MemoryMapLib.c
	gcc -MMD -MF -g -Os -fshort-wchar -fno-builtin -fno-strict-aliasing -Wall -Werror -Wno-array-bounds -fno-common -ffunction-sections -fdata-sections -DSTRING_ARRAY_NAME=BaseLibStrings -m32 -march=i586 -malign-double -fno-stack-protector -fno-asynchronous-unwind-tables -Wno-address -fno-pic -fno-pie -flto -Os -D DISABLE_NEW_DEPRECATED_INTERFACES -Wno-unused-but-set-variable -c -o $(WORKSPACE)../Build/OUTPUT/MemoryMapLib.o $(WORKSPACE)MemoryMapLib.c

$(OUTPUT_DIR)/QuickSort.o : $(MAKE_FILE)
$(OUTPUT_DIR)/QuickSort.o : $(WORKSPACE)QuickSort.c
	gcc -MMD -MF -g -Os -fshort-wchar -fno-builtin -fno-strict-aliasing -Wall -Werror -Wno-array-bounds -fno-common -ffunction-sections -fdata-sections -DSTRING_ARRAY_NAME=BaseLibStrings -m32 -march=i586 -malign-double -fno-stack-protector -fno-asynchronous-unwind-tables -Wno-address -fno-pic -fno-pie -flto -Os -D DISABLE_NEW_DEPRECATED_INTERFACES -Wno-unused-but-set-variable -c -o $(WORKSPACE)../Build/OUTPUT/QuickSort.o $(WORKSPACE)QuickSort.c

$(OUTPUT_DIR)/LinuxUniversalPayloadEntry.o : $(MAKE_FILE)
$(OUTPUT_DIR)/LinuxUniversalPayloadEntry.o : $(WORKSPACE)LinuxUniversalPayloadEntry.c
	gcc -MMD -MF -g -Os -fshort-wchar -fno-builtin -fno-strict-aliasing -Wall -Werror -Wno-array-bounds -fno-common -ffunction-sections -fdata-sections -DSTRING_ARRAY_NAME=BaseLibStrings -m32 -march=i586 -malign-double -fno-stack-protector -fno-asynchronous-unwind-tables -Wno-address -fno-pic -fno-pie -flto -Os -D DISABLE_NEW_DEPRECATED_INTERFACES -Wno-unused-but-set-variable -c -o $(WORKSPACE)../Build/OUTPUT/LinuxUniversalPayloadEntry.o $(WORKSPACE)LinuxUniversalPayloadEntry.c

$(OUTPUT_DIR)/Hob.o : $(MAKE_FILE)
$(OUTPUT_DIR)/Hob.o : $(WORKSPACE)Hob.c
	gcc -MMD -MF -g -Os -fshort-wchar -fno-builtin -fno-strict-aliasing -Wall -Werror -Wno-array-bounds -fno-common -ffunction-sections -fdata-sections -DSTRING_ARRAY_NAME=BaseLibStrings -m32 -march=i586 -malign-double -fno-stack-protector -fno-asynchronous-unwind-tables -Wno-address -fno-pic -fno-pie -flto -Os -D DISABLE_NEW_DEPRECATED_INTERFACES -Wno-unused-but-set-variable -c -o $(WORKSPACE)../Build/OUTPUT/Hob.o $(WORKSPACE)Hob.c

$(OUTPUT_DIR)/LinuxUniversalPayloadEntry.lib : $(OBJECT_FILES)
	gcc-ar cr $(WORKSPACE)../Build/OUTPUT/LinuxUniversalPayloadEntry.lib $(OBJECT_FILES)

$(DEBUG_DIR)/LinuxUniversalPayloadEntry.elf : $(MAKE_FILE)
$(DEBUG_DIR)/LinuxUniversalPayloadEntry.elf : $(STATIC_LIBRARY_FILES)
	gcc -o $(WORKSPACE)../Build/DEBUG/LinuxUniversalPayloadEntry.elf -nostdlib -Wl,--entry,_ModuleEntryPoint -u _ModuleEntryPoint -flto -Os -Wl,-m,elf_i386,--oformat=elf32-i386 -g -Os -fshort-wchar -fno-builtin -fno-strict-aliasing -Wall -Werror -Wno-array-bounds -fno-common -ffunction-sections -fdata-sections -DSTRING_ARRAY_NAME=LinuxUniversalPayloadEntryStrings -m32 -Wl,--start-group,$(STATIC_LIBRARY_FILES),--end-group -march=i586 -malign-double -fno-stack-protector -fno-asynchronous-unwind-tables -Wno-address -fno-pic -fno-pie -flto -Os -D DISABLE_NEW_DEPRECATED_INTERFACES -Wno-unused-but-set-variable 
	echo $(WORKSPACE)../Build/DEBUG/LinuxUniversalPayloadEntry.elf

$(OUTPUT_DIR)/InternalSwitchStack.o : $(MAKE_FILE)
$(OUTPUT_DIR)/InternalSwitchStack.o : $(WORKSPACE)InternalSwitchStack.nasm
	nasm -f elf32 -o $(WORKSPACE)../Build/OUTPUT/InternalSwitchStack.o $(WORKSPACE)InternalSwitchStack.nasm


$(OUTPUT_DIR)/LoadLinux.o : $(MAKE_FILE)
$(OUTPUT_DIR)/LoadLinux.o : $(WORKSPACE)LoadLinux.nasm
	nasm -f elf32 -o $(WORKSPACE)../Build/OUTPUT/LoadLinux.o $(WORKSPACE)LoadLinux.nasm

#
# clean all intermediate files
#
clean:
	rm -r -f $(OUTPUT_DIR)
	rm -f AutoGenTimeStamp

#
# clean all generated files
#
cleanall:
	rm -r -f $(DEBUG_DIR)
	rm -r -f $(OUTPUT_DIR)
	rm -f *.pdb *.idb > NUL 2>&1
	rm -f AutoGenTimeStamp


#
# clean all dependent libraries built
#
cleanlib:
	@cd $(MODULE_BUILD_DIR)