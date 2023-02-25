LIBNAME := s21_decimal
# end directories
BUILD := build
OUT := output
# paths
SRC := lib
TST := testing
INC := .# public header
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
OBJS = $(SRCS:$(SRC)/%.c=$(OBJ)/%.o) $(UTLS:$(UTL)/%.c=$(OBJ)/%.o)
INCS := $(INC) $(UTL)
HDRS := $(shell find $(INCS) -name *.h)
# specifications detection
KERNEL := $(shell uname -s)
DISTRO := $(shell cat /etc/os-release 2>/dev/null | grep -o '^NAME="[^"]*' | sed 's/NAME="//g')
ARCHITECTURE := $(shell uname -m)
ifeq ($(KERNEL), Darwin)
	OS := macOS
else ifeq ($(DISTRO), Ubuntu)
	OS := Ubuntu
else
	OS := Alpine
endif
# compilation parameters
CC := gcc
WFLAGS := -Wall -Werror -Wextra
CFLAGS := $(WFLAGS) -std=c11
LDFLAGS := $(INCS:%=-I%)
LDLIBS := -L$(LIB) -l$(LIBNAME)
ifeq ($(OS), Alpine)
	CFLAGS += -g
endif
SHELL := /bin/bash
# macros
RM := rm -rf
MK := mkdir -p
dir_guard = @$(MK) $(@D)
format_the_file = @clang-format -style=Google -i $@
# special targets and variables
ifneq ($(OS), Alpine)
	MAKEFLAGS += --no-print-directory
endif
.SECONDARY:# keeps all intermediate files (otherwise, they are automatically deleted)
.SUFFIXES:# cleans-up debug info
.PHONY: all test gcov_report clean   re lib memcheck linter linter_check miniverter docker
.DEFAULT_GOAL := all
export# exports environment variables to subshells and scripts


clean_build:
	$(RM) $(BUILD)
	$(RM) $(OUT)/*.a
	$(RM) *.a

re: clean
	$(MAKE) all

linter:
	clang-format -style=Google -i $(shell find . -type f -name '*.h' -o -name '*.c' -o -name '*.cs')
linter_check:
	clang-format -style=Google -n $(shell find . -type f -name '*.h' -o -name '*.c' -o -name '*.cs')

docker:
ifeq (, $(shell which docker))
	$(error "Docker" should be installed)
endif
	$(MAKE) clean_build
	bash $(SCRIPTS)/run_docker_image.sh