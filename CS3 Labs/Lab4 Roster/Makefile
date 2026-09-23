CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic

PROCEDURAL := classSchedule.exe
OBJECT := classScheduleObject.exe
INPUTS := cs1.txt cs2.txt cs3.txt cs4.txt dropouts.txt
EXPECTED := classScheduleTestOutput.txt

SHELL := cmd.exe
.SHELLFLAGS := /C

.PHONY: all build run run-procedural run-object test clean

all: build

build: $(PROCEDURAL) $(OBJECT)

$(PROCEDURAL): classSchedule.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJECT): classScheduleObject.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run: run-procedural

run-procedural: $(PROCEDURAL)
	.\$(PROCEDURAL) $(INPUTS)

run-object: $(OBJECT)
	.\$(OBJECT) $(INPUTS)

test: build
	.\$(PROCEDURAL) $(INPUTS) > classScheduleResult.txt
	.\$(OBJECT) $(INPUTS) > classScheduleObjectResult.txt
	fc classScheduleResult.txt $(EXPECTED)
	fc classScheduleObjectResult.txt $(EXPECTED)
	@echo All tests passed.

clean:
	@if exist "$(PROCEDURAL)" del /Q "$(PROCEDURAL)"
	@if exist "$(OBJECT)" del /Q "$(OBJECT)"
	@if exist "classScheduleResult.txt" del /Q "classScheduleResult.txt"
	@if exist "classScheduleObjectResult.txt" del /Q "classScheduleObjectResult.txt"
