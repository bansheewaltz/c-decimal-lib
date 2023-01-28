all: compile run

# mono framework should be installed
compile:
	mcs tester.cs
	gcc invocation_sample.c -o test
run:
	./test
