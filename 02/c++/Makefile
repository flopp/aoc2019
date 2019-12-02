prog: prog.cpp
	g++ -o $@ -std=c++11 -O2 -Wall -Wextra $<

puzzle1: prog
	./prog $@ 12 02 < ../input.txt

puzzle2: prog
	./prog $@ 19690720 < ../input.txt

format:
	clang-format -i prog.cpp
