prog: prog.cpp
	g++ -o $@ -std=c++17 -O2 -Wall -Wextra $<

test1: prog
	@echo "expected: 179"
	@./prog 10 < ../test1.txt

test2: prog
	@echo "expected: 1940"
	@./prog 100 < ../test2.txt

puzzle1: prog
	@./prog 1000 < ../input.txt

format:
	clang-format -i prog.cpp
