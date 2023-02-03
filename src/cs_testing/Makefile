SRC := test.cs
SRC := test.cs
EXE := $(SRC:%.cs=%.exe)
TST := test_cases
LDFLAGS := $(shell pkg-config --cflags --libs check)
RM := rm -rf
CLS := printf "\033c"

# mono framework should be installed
all: clean compile run
clean:
	@$(RM) *.exe
	@$(RM) $(TST)
compile:
	@mkdir -p $(TST)
	@mcs $(SRC)
run:
	@mkdir -p test_cases
	@mono $(EXE)

style:
	clang-format -style=Google -i *.{c,cs}


FNAME := mod
checkmk:
	checkmk clean_mode=1 $(FNAME)_test.check > $(FNAME)_test.c
	@$(MAKE) style
check: checkmk
	$(CLS)
	gcc $(LDFLAGS) -o $(FNAME) $(FNAME).c $(FNAME)_test.c ../utils.c
	@./$(FNAME)
