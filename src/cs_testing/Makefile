SRC := test.cs
SRC := test.cs
EXE := $(SRC:%.cs=%.exe)
TST := test_cases
LDFLAGS := $(shell pkg-config --cflags --libs check)
RM := rm -rf
CLS := printf "\033c"

# mono framework should be installed
all: clean generate
clean:
	@$(RM) *.exe *_test *_test.c
generate:
	@mcs $(SRC)
	@mono $(EXE)

style:
	clang-format -style=Google -i *.{c,cs}
# style:
# 	cp ../../materials/linters/.clang-format .
# 	clang-format -n *.{c,h,cs} || true
# 	clang-format -i *.{c,h,cs} || true


FNAME := div
checkmk:
	checkmk clean_mode=1 $(FNAME)_test.check > $(FNAME)_test.c
	@$(MAKE) style
check: checkmk
	$(CLS)
	@gcc $(LDFLAGS) -o $(FNAME)_test s21_$(FNAME).c $(FNAME)_test.c ../utils.c
	time ./$(FNAME)_test
