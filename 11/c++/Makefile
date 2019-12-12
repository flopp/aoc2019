prog: prog.cpp
	g++ -o $@ -std=c++11 -O2 -Wall -Wextra $<

puzzle: prog
	./prog < ../input.txt

format:
	clang-format -i prog.cpp
