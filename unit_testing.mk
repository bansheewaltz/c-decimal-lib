CONFIG := unit_testing
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
COV_REP := $(REPORTS)/coverage_report
COV_INF := $(BLD)/coverage_info
# filenames specification
TST_REPORT := $(REPORTS)/tests_report.txt
COV_REPORT := gcov_report.html
TEST_GEN := $(BIN)/test_generator.exe
# project files
T_SRCS := $(SRCS:$(SRC)/s21_%.c=$(SRC_B)/%_test.c)
T_EXES := $(SRCS:$(SRC)/s21_%.c=$(BIN)/%_test)


test: $(T_EXES)
	bash $(SCRIPTS)/format_report.sh $(REPORTS)
	cat $(TST_REPORT)
	@printf '=%.0s' {1..97} | xargs printf '%s\n' >> $(REPORTS)/tests_report_raw.txt

$(BIN)/%_test: $(OBJ)/s21_%.o $(OBJ)/utils.o $(OBJ)/%_test.o | $(COV_INF) $(REPORTS) $(CASES_B)
	$(dir_guard)
	$(CC) --coverage $^ $(LIBS) $(LDFLAGS) -o $@
	(cd $(BIN) && ./$(@F) && echo) >> $(REPORTS)/tests_report_raw.txt
	-mv $(OBJ)/*.{gcno,gcda} $(COV_INF)

$(OBJ)/s21_%.o: $(SRC)/s21_%.c | $(OBJ)
	$(CC) -c $(INCS) $(CFLAGS) --coverage $< -o $@

$(OBJ)/utils.o: $(UTLS) | $(OBJ)
	$(CC) -c $(INCS) $(CFLAGS) $< -o $@

$(OBJ)/%_test.o: INCS += -I $(T_INC)# speicifies a target-specific variable
$(OBJ)/%_test.o: $(SRC_B)/%_test.c | $(OBJ)
	$(CC) -c $(INCS) $(LDFLAGS) $< -o $@

$(SRC_B)/%_test.c: $(CHK_B)/%_test.check
	$(dir_guard)
	checkmk clean_mode=1 $< > $@
	$(format_the_file)

$(CHK_B)/%_test.check: $(CHK_T)/%_test.check $(CASES_T)/bin/%.bin
	$(dir_guard)
	cp $< $(@D)
ifeq ($(OS), Darwin)
	sed -i "" 's/PLACEHOLDER/$(shell expr $(shell wc -l < $(shell find $(CASES_T)/txt -name $*.txt)) - 2)/g' $@
else
	sed -i 's/PLACEHOLDER/$(shell expr $(shell wc -l < $(shell find $(CASES_T)/txt -name $*.txt)) - 2)/g' $@	
endif


# directories creation
$(OBJ):
	@$(MK) $@
$(COV_INF):
	@$(MK) $@
$(REPORTS):
	@$(MK) $@
$(CASES_B):
	@$(MK) $@
	cp $(CASES_T)/bin/* $@

# generates tests with program written in C#
## "mono" framework should be installed for compilation and execution
generate_tests: $(TEST_GEN)
$(BIN)/%.exe: $(SRC_T)/%.cs
	$(dir_guard)
	mcs $< -out:$@
	mono $@


gcov_report: $(T_EXES)
	@$(MK) $(COV_REP)
ifeq ($(OS), Darwin)
	gcovr --html-details --html-self-contained -o $(COV_REP)/$(COV_REPORT) $(COV_INF)
	open $(COV_REP)/$(COV_REPORT)
else
	#???
endif

.PHONY: test generate_tests gcov_report