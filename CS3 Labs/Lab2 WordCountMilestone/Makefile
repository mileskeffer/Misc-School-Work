target = wordCountMilestone
source = main.cpp
compiler = g++
flags = -std=c++17

build:
	$(compiler) $(flags) $(source) -o $(target)

run: build
	./$(target) test.txt

clean:
	rm -f $(target)

.PHONY: build run clean
