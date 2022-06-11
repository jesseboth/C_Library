CC := gcc
CFLAGS := -g -Wall -std=c99 -fPIC -D_DEFAULT_SOURCE
CODE := src/main.o src/util.o
TESTS :=


all: a.out run

a.out: $(CODE)
	$(CC) -o $@ $^ $(CFLAGS)

run:
	@echo " "
	@./a.out
	@echo " "

debug: $(CODE)
	$(CC) -o $@ $^ $(CFLAGS)
	@gdb ./a.out

test: $(TESTS) $(NEWTESTS)
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
	rm -f src/*.o tests/*.o *~ src/*~ tests/*~

# See previous assignments for a description of .PHONY
.PHONY: all clean