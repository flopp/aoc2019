prog: prog.cpp
	g++ -o $@ -std=c++11 -O2 -Wall -Wextra $<

test1: prog
	@echo "Expected: 159"
	@./prog $@ < ../input-test1.txt
	@echo "Expected: 135"
	@./prog $@ < ../input-test2.txt

puzzle1: prog
	./prog $@ < ../input.txt

format:
	clang-format -i prog.cpp
