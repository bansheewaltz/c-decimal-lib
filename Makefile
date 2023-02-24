CONFIG := release
### many variables and targets are stored in a shared makefile
include shared.mk
# project files
INC0 := $(INC) $(UTL)
INCS := $(INC0:%=-I %)
HDRS := $(shell find $(INC0) -name *.h)
vpath %.c $(SRC)
export INCS


all:
	$(MAKE) lib
	$(MAKE) test
	$(MAKE) gcov_report
ifeq ($(OS), Linux)
	$(MAKE) memcheck
endif

lib: $(OUT)/lib$(LIBNAME).a

$(OUT)/lib%.a: %.a | $(OUT)
	mv $< $@

%.a: $(OBJS)
	$(MK) $(BIN)
	ar -rsc $@ $^
	@cp $@ $(SRC)
	@cp $@ $(BIN)

$(OBJ)/%.o: %.c $(HDRS)
	$(dir_guard)
	$(CC) -c $(INCS) $(CFLAGS) $< -o $@

$(OUT):
	$(MK) $@

test gcov_report: lib
	$(MAKE) -f unit_testing.mk $@


memcheck: lib
	$(MAKE) -s -f memcheck.mk $@

miniverter: clean
	cd materials/build && bash run.sh