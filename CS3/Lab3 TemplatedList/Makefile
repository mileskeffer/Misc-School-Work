CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic

TARGET := testCollection.exe
SOURCE := testCollection.cpp
HEADERS := testCollection.hpp list.hpp

SHELL := cmd.exe
.SHELLFLAGS := /C

.PHONY: all build run test clean

all: build

build: $(TARGET)

$(TARGET): $(SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	.\$(TARGET)

test: $(TARGET)
	.\$(TARGET) > result.txt

clean:
	@if exist "$(TARGET)" del /Q "$(TARGET)"
	@if exist "result.txt" del /Q "result.txt"
