# file to build
sources = main.cpp

# file to output
target = diff

compiler = g++

# compiler flags
flags = -std=c++17

# build file
build:
	$(compiler) $(flags) $(sources) -o $(target)

# run the test saving its result to a file
run: build
	./$(target) file1.txt file2.txt > test1Result.txt

# "build", "run", "clean", and "test" are not produced by make
.PHONY: build run clean test

# delete built file
clean:
	rm -f $(target)

# check if the program output matches the desired output
test: run
	diff -C 3 test1Result.txt test1Solution.txt
	@echo tests passed great job
