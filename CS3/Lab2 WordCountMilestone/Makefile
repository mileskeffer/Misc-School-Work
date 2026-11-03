target = wordCountMilestone.exe
source = main.cpp
compiler = g++
flags = -std=c++17

build:
	$(compiler) $(flags) $(source) -o $(target)

run: build
	$(target) test.txt

clean:
	if exist $(target) del /Q $(target)

.PHONY: build run clean
