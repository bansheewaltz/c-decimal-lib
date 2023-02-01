SRC := test_generator.cs
EXE := $(SRC:%.cs=%.exe)
LDFLAGS = -D_THREAD_SAFE -I/opt/homebrew/Cellar/check/0.15.2/include \
		      	  -L/opt/homebrew/Cellar/check/0.15.2/lib -lcheck

# mono framework should be installed
all: clean compile run
clean:
	rm -f $(EXE)
	rm -f ./test_cases/*
compile:
	mkdir -p test_cases
	mcs $(SRC)
run:
	mono $(EXE)

style:
	clang-format -style=Google -i *.cs