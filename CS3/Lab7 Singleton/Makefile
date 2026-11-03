CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic
TARGETS := singletonLogger.exe templateMethodGames.exe

SHELL := cmd.exe
.SHELLFLAGS := /C

.PHONY: all clean

all: $(TARGETS)

singletonLogger.exe: singletonLogger.cpp Logger.hpp
	$(CXX) $(CXXFLAGS) singletonLogger.cpp -o $@

templateMethodGames.exe: templateMethodGames.cpp
	$(CXX) $(CXXFLAGS) templateMethodGames.cpp -o $@

clean:
	@if exist singletonLogger.exe del /Q singletonLogger.exe
	@if exist templateMethodGames.exe del /Q templateMethodGames.exe
