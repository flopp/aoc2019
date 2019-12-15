prog: prog.cpp
	g++ -o $@ -std=c++17 -O2 -Wall -Wextra $<

format:
	clang-format -i prog.cpp

test1: prog
	@echo "expected: 31"
	@./prog < ../test1.txt
test2: prog
	@echo "expected: 165"
	@./prog < ../test2.txt
test3: prog
	@echo "expected: 13312"
	@./prog < ../test3.txt
test4: prog
	@echo "expected: 180697"
	@./prog < ../test4.txt
test5: prog
	@echo "expected: 2210736"
	@./prog < ../test5.txt

puzzle1: prog
	@./prog < ../input.txt

