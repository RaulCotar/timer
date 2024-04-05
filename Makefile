MAKEFLAGS += --no-builtin-rules --no-builtin-variables
undefine CC
SRC := $(wildcard *.c)
CC ?= clang
ARG ?=

.PHONY: run
run: timer
	./$< $(ARG)

timer: $(SRC) $(wildcard *.h) compile_flags.txt Makefile
	$(CC) $$(grep '^[^#]' compile_flags.txt) $(SRC) -o $@

.PHONY: delme
delme:
	$(CC) $$(grep '^[^#]' compile_flags.txt) delme.c -o $@
	./$@
	rm $@

.PHONY: clean
clean:
	rm -f timer
