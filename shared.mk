LIBNAME := s21_decimal
# end directories
BUILD := build
OUT := output
# paths
SRC := src
TST := testing
INC := include# public header
UTL := $(SRC)/utils
BLD = $(BUILD)/$(CONFIG)
OBJ = $(BLD)/obj
BIN = $(BLD)/bin
LIB = $(OUT)
REPORTS := $(OUT)
SCRIPTS := $(TST)/scripts
# project files
SRCS := $(wildcard $(SRC)/*.c)
UTLS := $(wildcard $(UTL)/*.c)
OBJS = $(SRCS:$(SRC)/%.c=$(OBJ)/%.o)
# compilation parameters
CC := gcc
WFLAGS := -Wall -Werror -Wextra
CFLAGS := $(WFLAGS) -std=c11
LIBS := -L$(LIB) -l$(LIBNAME)
# platform-dependent linker parameters
OS := $(shell uname -s)
ifeq ($(OS), Darwin)
	LDFLAGS := -lcheck
	ifeq ($(shell uname -p), arm)# ARM-based Macs
		LDFLAGS := $(shell pkg-config --cflags --libs check)
	endif
else ifeq ($(OS),Linux)
	LDFLAGS := $(shell pkg-config --cflags --libs check)
endif
# macros
RM := rm -rf
MK := mkdir -p
dir_guard = @$(MK) $(@D)
format_the_file = @clang-format -style=Google -i $@
# special targets and variables
.SECONDARY:# keeps all intermediate files (otherwise, they are automatically deleted)
.SUFFIXES:# cleans-up debug info
.PHONY: test gcov_report clean   all re memcheck linter miniverter
.DEFAULT_GOAL := all
MAKEFLAGS += --no-print-directory
export# exports environment variables to subshells and scripts


clean:
	$(RM) $(BUILD)
	$(RM) $(OUT)
	$(RM) $(SRC)/*.a
	@$(RM) *.a

re: clean
	$(MAKE) all

linter:
	clang-format -style=Google -i $(shell find . -type f -name '*.h' -o -name '*.c' -o -name '*.cs')

docker: clean
	bash $(SCRIPTS)/run_docker_image.sh