CONFIG := release
### many variables and targets are stored in a shared makefile
include Makefile.common
# searches path for .c files
vpath %.c $(SRC) $(UTL)
export


all:
	$(MAKE) lib
	$(MAKE) test
	$(MAKE) gcov_report
ifeq ($(OS), Linux)
	$(MAKE) memcheck
endif

s21_decimal.a: $(OBJS) | $(LIB)
	$(MK) $(BIN)
	ar -rsc $@ $^

$(OBJ)/%.o: %.c $(HDRS)
	$(dir_guard)
	$(CC) -c $(LDFLAGS) $(CFLAGS) $< -o $@

lib: $(LIB)/libs21_decimal.a
$(LIB)/libs21_decimal.a: s21_decimal.a | $(LIB)
	cp $< $@

$(LIB):
	$(MK) $@

clean:
	$(RM) $(BUILD)
	$(RM) $(REPORTS)
	$(RM) *.a

test gcov_report: lib
	@$(MAKE) -s -f Makefile.unit_testing $@

memcheck: lib
	$(MAKE) -s -f Makefile.memcheck $@

miniverter: clean
	cd $(SCRIPTS) && bash run_miniverter_test.sh