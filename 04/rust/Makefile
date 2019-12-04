.PHONY: all clean

all: puzzle

clean:
	@rm -f prog

prog: prog.rs
	@rustc -O -o $@ $<

puzzle: prog
	@./prog
