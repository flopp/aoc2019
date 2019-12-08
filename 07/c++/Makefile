prog: prog.cpp
	g++ -o $@ -std=c++11 -O2 -Wall -Wextra $<

test1: prog
	@echo "expected: 43210 => 43210"
	@./prog < ../test1.txt

test2: prog
	@echo "expected: 01234 => 54321"
	@./prog < ../test2.txt

puzzle1: prog
	./prog < ../input.txt

puzzle2: prog
	./prog 5 < ../input.txt

format:
	clang-format -i prog.cpp
