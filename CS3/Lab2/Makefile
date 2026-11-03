# file to build
sources = wordListMain.cpp wordListDefinitions.cpp

# case sensitive or insensitive
case = insensitive

# file to output
target = wordCount.exe

compiler = g++

# compiler flags
flags = -std=c++17

# build file
build:
	$(compiler) $(flags) $(sources) -o $(target)

# run both tests saving their results to files
run: build
	$(target) file1.txt > test1Result.txt

# "clean" and "test" are not produced by make
.PHONY: clean test

# delete built file
clean:
	if exist $(target) del /Q $(target)

sensitive:
	fc /N test1Result.txt caseSensitiveSolution.txt

insensitive:
	fc /N test1Result.txt caseInsensitiveSolution.txt

# check if the programs output matches the desired output
test: run $(case)
	$(compiler) $(flags) testWordList.cpp wordListDefinitions.cpp -o testClass.exe
	@echo
	testClass.exe
	@echo tests passed great job
