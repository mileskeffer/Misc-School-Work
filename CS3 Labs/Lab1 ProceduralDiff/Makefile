#modified make file with the first test and solution
# file to build
sources = main.cpp

# file to output
target = diff.exe

compiler = g++

# compiler flags
flags = -std=c++17

# build file
build:
	$(compiler) $(flags) $(sources) -o $(target)

# run both tests saving their results to files
run: build
	.\$(target) file1.txt file2.txt > test1Result.txt

# "clean" and "test" are not produced by make
.PHONY: build run clean test

# delete built file
clean:
	if exist $(target) del /Q $(target)

# check if the programs output matches the desired output
test: run
	fc test1Result.txt test1Solution.txt
	@echo tests passed great job


