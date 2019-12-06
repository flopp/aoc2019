prog: prog.cpp
	g++ -o $@ -std=c++11 -O2 -Wall -Wextra $<

puzzle: prog
	@./prog < ../input.txt

test1: prog
	@echo "expected: 42 / ?"
	@./prog < ../test1.txt

test2: prog
	@echo "expected: ? / 4"
	@./prog < ../test2.txt

format:
	clang-format -i prog.cpp
