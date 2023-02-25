CONFIG := release
### many variables and targets are stored in a shared makefile
include shared.mk
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

s21_decimal.a: $(OBJS) | $(OUT)
	$(MK) $(BIN)
	ar -rsc $@ $^

$(OBJ)/%.o: %.c $(HDRS)
	$(dir_guard)
	$(CC) -c $(LDFLAGS) $(CFLAGS) $< -o $@

$(OUT):
	$(MK) $@

lib: $(OUT)/libs21_decimal.a

$(OUT)/libs21_decimal.a: s21_decimal.a | $(OUT)
	cp $< $@

clean:
	$(RM) $(BUILD)
	$(RM) $(OUT)
	$(RM) *.a

test gcov_report: lib
	$(MAKE) -f unit_testing.mk $@

memcheck: lib
	$(MAKE) -f memcheck.mk $@

miniverter: clean
	cd $(SCRIPTS) && bash run_miniverter.sh