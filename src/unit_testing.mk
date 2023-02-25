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
TST_REPORT_R := $(REPORTS)/tests_report_raw.txt
COV_REPORT := gcov_report.html
TEST_GEN := $(BIN)/test_generator.exe
# project files
T_SRCS := $(SRCS:$(SRC)/s21_%.c=$(SRC_B)/%_test.c)
T_EXES := $(SRCS:$(SRC)/s21_%.c=$(BIN)/%_test)
# compilation parameters
CCOVFLAGS := --coverage
LDFLAGS += $(shell pkg-config --cflags check)
LDLIBS += $(shell pkg-config --libs check)


test: $(T_EXES)
	bash $(SCRIPTS)/format_report.sh $(TST_REPORT_R) $(TST_REPORT)
	@echo '\n\n' && cat $(TST_REPORT)
	@printf '=%.0s' {1..97} | xargs printf '%s\n' >> $(TST_REPORT_R)

$(BIN)/%_test: $(OBJ)/s21_%.o $(OBJ)/%_test.o | $(COV_INF) $(REPORTS) $(CASES_B)
	$(dir_guard)
	$(CC) $(CCOVFLAGS) $^ $(LDLIBS) -o $@
	(cd $(BIN) && ./$(@F) && echo) >> $(TST_REPORT_R)
	-mv $(OBJ)/*.{gcno,gcda} $(COV_INF)

$(OBJ)/s21_%.o: $(SRC)/s21_%.c | $(OBJ)
	$(CC) -c $(LDFLAGS) $(CFLAGS) $(CCOVFLAGS) $< -o $@

$(OBJ)/%_test.o: LDFLAGS += -I $(T_INC)# speicifies a target-specific variable
$(OBJ)/%_test.o: $(SRC_B)/%_test.c | $(OBJ)
	$(CC) -c $(LDFLAGS) $< -o $@

$(SRC_B)/%_test.c: $(CHK_B)/%_test.check
	$(dir_guard)
	checkmk clean_mode=1 $< > $@
	$(format_the_file)

$(CHK_B)/%_test.check: $(CHK_T)/%_test.check $(CASES_T)/bin/%.bin
	$(dir_guard)
	cp $< $(@D)
ifeq ($(OS), macOS)
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

# generates tests with a program written in C#
## "mono" framework should be installed for compilation and execution
generate_tests:$(TEST_GEN)
$(BIN)/%.exe: $(SRC_T)/%.cs
ifeq (, $(shell which mono))
	$(error "mono" framework should be installed)
endif
	$(dir_guard)
	mcs $< -out:$@
	mono $@


gcov_report: $(T_EXES)
ifeq (, $(shell which gcovr))
	$(error "gcovr" tool should be installed)
endif
	@$(MK) $(COV_REP)
	gcovr --html-details --html-self-contained -o $(COV_REP)/$(COV_REPORT) $(COV_INF)
ifeq ($(OS), macOS)
	open $(COV_REP)/$(COV_REPORT)
endif


.PHONY: test generate_tests gcov_report