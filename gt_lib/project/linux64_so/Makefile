# Copyright (c) LEADERG Co., Ltd. All rights reserved.
# Free of charge for noncommercial purposes.
# Please contact LEADERG to buy licenses for commercial purposes. (http://www.leaderg.com/license)
# THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY.


ROOT = /home/mingta/RobotSDK
OUTPUT = libgt.so
OUTPUTDIR = $(ROOT)/gt_lib/project/linux64_so


SOURCES = \
../../src/gt_array.c \
../../src/gt_buffer.c \
../../src/gt_color.c \
../../src/gt_config_parser.c \
../../src/gt_fifo.c \
../../src/gt_file.c \
../../src/gt_hash.c \
../../src/gt_hash_map.c \
../../src/gt_http.c \
../../src/gt_httpd.c \
../../src/gt_lib.c \
../../src/gt_lock.c \
../../src/gt_log.c \
../../src/gt_matrix.c \
../../src/gt_mem.c \
../../src/gt_runtime.c \
../../src/gt_semaphore.c \
../../src/gt_timer_us.c \
../../src/gt_stack.c \
../../src/gt_string.c \
../../src/gt_utf8.c \
../../src/gt_matrix.c \
../../src/gt_template.c \
../../../third_party/hash/sha1.c \
../../../third_party/hash/sha256.c \
../../../third_party/hash/md5c.c



# Include directory settings
IDIR        := -I$(ROOT)/gt_lib/include -I$(OUTPUTDIR) -I$(ROOT)/third_party/hash


THIS_MAKEFILE_PATH:=$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
THIS_MAKEFILE_DIR:=$(shell cd $(dir $(THIS_MAKEFILE_PATH));pwd)
THIS_MAKEFILE:=$(notdir $(THIS_MAKEFILE_PATH))
ROOT_DIR:=$(shell cd $(dir $(ROOT));pwd)


# Compiler settings
TOOLCHAIN_BIN_PATH     =
TOOLCHAIN_INCLUDE_PATH = /usr/include
TOOLCHAIN_LIB_PATH     = /usr/lib
CROSS_COMPILE          =
NVCC_PATH              = /usr/local/cuda-5.0


# Compiler flags
CFLAGS          += -O2 -Wall -I$(THIS_MAKEFILE_DIR) $(IDIR) -I$(TOOLCHAIN_INCLUDE_PATH) -fPIC -shared
CFLAGS          += -mmmx -msse -msse2
# CFLAGS          += -std=c99 
CXXFLAGS        += -static -O2 -Wall -I$(THIS_MAKEFILE_DIR) -I$(IDIR) -I$(IDIR) -I$(TOOLCHAIN_INCLUDE_PATH)
CXXFLAGS        += -mmmx -msse -msse2 
NVCCFLAGS       += -O2 -I$(THIS_MAKEFILE_DIR) -I$(IDIR) -I$(TOOLCHAIN_INCLUDE_PATH)
LFLAGS          += -L$(TOOLCHAIN_LIB_PATH)
EXTRA_CFLAGS    += 
EXTRA_CXXFLAGS  += 
EXTRA_NVCCFLAGS += 
EXTRA_LFLAGS    +=
EXTRA_LIBS      += -lpthread -lm


# Compiler commands
CC      = $(TOOLCHAIN_BIN_PATH)$(CROSS_COMPILE)gcc
CXX     = $(TOOLCHAIN_BIN_PATH)$(CROSS_COMPILE)g++
AR      = $(TOOLCHAIN_BIN_PATH)$(CROSS_COMPILE)ar
LD      = $(TOOLCHAIN_BIN_PATH)$(CROSS_COMPILE)ld
STRIP   = $(TOOLCHAIN_BIN_PATH)$(CROSS_COMPILE)strip
MAKE    = make
NVCC    = $(NVCC_PATH)/bin/nvcc


# File management
CP := cp -u
RM := rm -f


APP  = $(OUTPUT)


# Force sh to be a shell command interpreter
SHELL ?= /bin/bash


##include $(ROOT)/defs.mk


.PHONY  : clean


# Switch to silent mode
ifndef NOSILENT
.SILENT:
endif


OBJECTS_TEMP = $(SOURCES:.c=.o)
OBJECTS_TEMP2 = $(OBJECTS_TEMP:.cu=.o)
OBJECTS = $(OBJECTS_TEMP2:.cpp=.o)


all :  $(APP)


$(APP)  : $(OBJECTS)
	@echo "Target is APP : libgt.so"
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(EXTRA_LFLAGS) $^  $(CLIBS) $(EXTRA_LIBS) -o $(OUTPUTDIR)/$(OUTPUT)


# The way to compile c source files.
%.o : %.c
	@echo "Compiling C source file: $<"
	$(CC) $(EXTRA_CFLAGS) $(CFLAGS) -c $< -o $@     


# The way to compile cpp source files.
%.o : %.cpp
	@echo "Compiling CXX source file: $<"
	$(CXX) $(EXTRA_CXXFLAGS) $(CXXFLAGS) -c $< -o $@
	
	
# The way to compile cuda source files.
%.o : %.cu
	@echo "Compiling CUDA source file: $<"
	$(NVCC) $(EXTRA_NVCCFLAGS) $(NVCCFLAGS) -c $< -o $@ 	  


clean :
	@echo "Cleaning files  $(OBJECTS) $(HEADERS) $(APP)"
	$(RM) $(OBJECTS)
	$(RM) $(APP)


run :
	$(OUTPUTDIR)/$(OUTPUT)
#	sudo $(OUTPUTDIR)/$(OUTPUT)


# Add your debug rules here
dbg:
	@echo "MAKE OPTIONS"
	@echo "Makefile       : $(THIS_MAKEFILE)"
	@echo "Makefile DIR   : $(THIS_MAKEFILE_DIR)"
	@echo "ROOT           : $(ROOT)"
	@echo "ROOT DIR       : $(ROOT_DIR)"
	@echo "Sources        : $(SOURCES)"
	@echo "Headers        : $(HEADERS)"
	@echo "Objects        : $(OBJECTS)"
	@echo "PATH           : $(PATH)"
	@echo "CC             : $(CC)"
	@echo "CFLAGS         : $(CFLAGS)"
	@echo "AR             : $(AR)"
