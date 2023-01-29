SRC := test_generator.cs
EXE := $(SRC:%.cs=%.exe)

# mono framework should be installed
all: clean compile run
clean:
	rm -f $(EXE)
	rm -f ./test_cases/*
compile:
	mcs $(SRC)
run:
	mono $(EXE)

style:
	clang-format -style=Google -i *.cs
