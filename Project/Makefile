CC := gcc
CFLAGS := -g -Wall -std=c99 -fPIC -D_DEFAULT_SOURCE -lm
CODE   := src/utility/util.o src/utility/geom.o src/utility/timer.o src/linked_lib.o
C_CODE := src/utility/util.c src/utility/geom.c src/utility/timer.c src/linked_lib.c


TESTS_UTIL := util~mem_cpy util~mem_clear
TESTS_LINKED := linkedlist~insert linkedlist~sort

TESTS := $(TESTS_UTIL) $(TESTS_LINKED)


all: a.out run

a.out: $(CODE) src/main.o
	$(CC) -o $@ $^ $(CFLAGS)

run:
	@echo " "
	@./a.out
	@echo " "

debug: $(CODE) src/main.o
	$(CC) -o $@ $^ $(CFLAGS)
	@gdb ./a.out

asm:
	$(CC) -S $(C_CODE)

test: $(TESTS)
	@echo
	@for test in $^; do                                   \
	    printf "Running %-30s: " "$$test";                \
	    (./$$test && echo "passed") || echo "failed";       \
	done
	@echo

# to create an object file from a source C file.
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
 

# This pattern will build any self-contained test file in tests/.  If
# your test file needs more support, you will need to write an explicit
# rule for it.
#
# To add a test, create a file called tests/testname.c that contains a
# main function and all of the relevant test code, then add the basename
# of the file (e.g., testname in this example) to TESTS, above.
%: tests/%.o $(CODE)
	$(CC) -o $@ $^

clean:
	rm -f $(TESTS) a.out
	rm -f src/*.o tests/*.o *~ src/*~ tests/*~  *.s

# See previous assignments for a description of .PHONY
.PHONY: all clean