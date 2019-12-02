prog: prog.cpp
	g++ -o $@ -std=c++11 -O2 -Wall -Wextra $<

test1: prog
	./prog $@

test2: prog
	./prog $@

puzzle1: prog
	./prog $@ < ../input.txt

puzzle2: prog
	./prog $@ < ../input.txt

format:
	clang-format -i prog.cpp
