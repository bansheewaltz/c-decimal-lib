SRC := test.cs
EXE := $(SRC:%.cs=%.exe)
LDFLAGS := $(shell pkg-config --cflags --libs check)

# mono framework should be installed
all: clean compile run
clean:
	@rm -f *.{exe,bin,txt}
compile:
	@mcs $(SRC)
run:
	@mono $(EXE)

style:
	-clang-format -style=Google -i *.cs

check: clean
	clear
	checkmk clean_mode=1 test.check > test.c
	gcc $(LDFLAGS) check.c test.c -o check
	./check