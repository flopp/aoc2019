prog: prog.cpp
	g++ -o $@ -std=c++11 -O2 -Wall -Wextra $<

test1: prog
	@echo "expected: 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99"
	@./prog < ../test1.txt

test2: prog
	@echo "expected: '16-digit number'"
	@./prog < ../test2.txt

test3: prog
	@echo "expected: 1125899906842624"
	@./prog < ../test3.txt

puzzle1: prog
	./prog 1 < ../input.txt

puzzle2: prog
	./prog 2 < ../input.txt

format:
	clang-format -i prog.cpp
