SRCS := $(wildcard *.[ch])

.PHONY: run
run: timer
	./$< -s1

timer: main.c $(SRCS) compile_flags.txt Makefile
	clang $$(grep '^[^#]' compile_flags.txt) $< -o $@

.PHONY: clean
clean:
	rm -f timer