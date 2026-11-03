CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic
TARGET := roster.exe
INPUTS := cs1.txt cs2.txt cs3.txt cs4.txt dropouts.txt

SHELL := cmd.exe
.SHELLFLAGS := /C

.PHONY: all run clean

all: $(TARGET)

$(TARGET): roster.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run: $(TARGET)
	.\$(TARGET) $(INPUTS)

clean:
	@if exist "$(TARGET)" del /Q "$(TARGET)"
