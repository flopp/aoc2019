prog: prog.cpp
	g++ -o $@ -std=c++11 -O2 -Wall -Wextra $<

puzzle1: prog
	./prog 1 < ../input.txt

puzzle2: prog
	./prog 5 < ../input.txt

format:
	clang-format -i prog.cpp
