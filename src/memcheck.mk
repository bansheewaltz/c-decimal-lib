CONFIG := memcheck
NUMBER_OF_TESTS := 10
### some variables and targets are stored in a shared makefile
include shared.mk
# paths
CHK_T := $(TST)/check/templates
CHK_B := $(BLD)/check
SRC_T := $(TST)/src
SRC_B := $(BLD)/src
T_INC := $(TST)/headers
CASES_T := $(TST)/cases
CASES_B := $(BLD)/bin/cases
SCRIPTS := $(TST)/scripts
# project files
T_SRCS := $(SRCS:$(SRC)/s21_%.c=$(SRC_B)/%_test.c)
T_EXES := $(SRCS:$(SRC)/s21_%.c=$(BIN)/%_test)
# compilation parameters
INCS += -I $(T_INC)
CFLAGS = -g
# VALGRIND_FLAGS := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s
VALGRIND_FLAGS := valgrind
CK_FORK=no
# filenames specification
REPORT := $(OUT)/memcheck_raw.txt
export# exports environment variables to subshells and scripts

# memcheck: $(REPORT)
# $(REPORT): $(T_EXES)
memcheck: $(T_EXES)
	# bash $(SCRIPTS)/format_memcheck.sh
	# @echo "\n\nCheck report at \"output\" folder"
$(BIN)/%_test: $(SRC)/s21_%.c $(SRC_B)/%_test.c | $(CASES_B) $(OUT)
	$(dir_guard)
	$(CC) $(INCS) $(CFLAGS) $^ $(LIBS) $(LDFLAGS) -o $@
	#(cd $(BIN) && echo && $(VALGRIND_FLAGS) ./$(@F)) 2>> $(REPORT)
	(cd $(BIN) && echo && $(VALGRIND_FLAGS) ./$(@F)) 2> >(grep "at exit")

$(SRC_B)/%_test.c: $(CHK_T)/%_test.check | $(CHK_B)
	$(dir_guard)
	sed 's/PLACEHOLDER/$(NUMBER_OF_TESTS)/g' $< > $(CHK_B)/$(<F)
	checkmk clean_mode=1 $(CHK_B)/$(<F) > $@
	$(format_the_file)

$(CHK_B):
	@$(MK) $@
$(OUT):
	@$(MK) $@
$(CASES_B):
	@$(MK) $@
	cp $(CASES_T)/bin/* $@


.PHONY: memcheck