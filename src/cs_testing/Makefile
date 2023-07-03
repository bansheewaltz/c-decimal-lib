# file pointers
LIBNAME := x_decimal
REPORT := error_report.txt
SRCS_CS := test_suite.cs
UTILS := ../utils.c
# compilation flags
LDFLAGS = # will be set based on OS variable
# paths
SRC := src
CHK := check
TST := test_cases
OBJ := obj
EXE := build
# project files
SRCS := $(wildcard *.c)
TESTSC := $(SRCS:x_%.c=$(SRC)/%_test.c)
TEXECS := $(SRCS:x_%.c=$(EXE)/%_test)
EXE_CS := $(SRCS_CS:%.cs=%.exe)
# platform dependent stuff
OS := $(shell uname -s)
ifeq ($(OS), Darwin)
	LDFLAGS = -lcheck
	ifeq ($(shell uname -p), arm) # M1+ Mac
		LDFLAGS = $(shell pkg-config --cflags --libs check)
	endif
else ifeq ($(OS),Linux)
	LDFLAGS = -lcheck -pthread -lcheck_pic -pthread -lrt -lm -lsubunit
endif
# macros
RM := rm -rf
CLS := printf "\033c"


all: generate
clean:
	$(RM) $(SRC)/*.c
	$(RM) $(EXE)/*
	$(RM) $(REPORT)
# generates tests in C#
## mono framework should be installed
generate:
	mcs $(SRCS_CS)
	mono $(EXE_CS)

style:
	clang-format -style=Google -i *.{c,cs} $(SRC)/*


# testing
test: $(TEXECS)
	$(RM) $(REPORT)
	$(foreach file, $(TEXECS), ./$(file) >> error_report.txt;)
	#code $(REPORT)
$(EXE)/%_test: $(SRC)/%_test.c
	gcc $(LDFLAGS) -o $@ $< x_$*.c ../utils.c	
$(SRC)/%.c: $(CHK)/%.check
	checkmk clean_mode=1 $< > $@

# testing of specified function
FNAME := from_float_to_decimal
testf:
	@$(CLS)
	@checkmk clean_mode=1 $(CHK)/$(FNAME)_test.check > $(SRC)/$(FNAME)_test.c
	@$(MAKE) style
	@cd $(SRC) && gcc $(LDFLAGS) -o ../$(EXE)/$(FNAME)_test ../x_$(FNAME).c $(FNAME)_test.c ../../utils.c
	@time $(EXE)/$(FNAME)_test
