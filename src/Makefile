CONFIG := release
### many variables and targets are stored in a shared makefile
include shared.mk
# project files
INC0 := $(INC) $(UTL)
INCS := $(INC0:%=-I%)
HDRS := $(shell find $(INC0) -name *.h)
vpath %.c $(SRC) $(UTL)
export


all:
	$(MAKE) lib
	$(MAKE) test
	$(MAKE) gcov_report
ifeq ($(OS), Linux)
	$(MAKE) memcheck
endif

lib: $(OUT)/lib$(LIBNAME).a | $(OUT)

$(OUT)/lib%.a: %.a | $(OUT)
	mv $< $@

%.a: $(OBJS) | $(OUT)
	$(MK) $(BIN)
	ar -rsc $@ $^
	@cp $@ $(OUT)

$(OBJ)/%.o: %.c $(HDRS)
	$(dir_guard)
	$(CC) -c -Ilib/include -Ilib/utils $(CFLAGS) $< -o $@

$(OUT):
	$(MK) $@

test gcov_report: lib
	$(MAKE) -f unit_testing.mk $@


memcheck: lib
	$(MAKE) -s -f memcheck.mk $@

miniverter: clean
	cd $(SCRIPTS) && bash run_miniverter.sh