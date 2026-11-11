# Miles Keffer
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic

.PHONY: all clean

all: coffeeShack

coffeeShack: coffeeShack.cpp drink.cpp drink.hpp
	$(CXX) $(CXXFLAGS) coffeeShack.cpp drink.cpp -o $@

clean:
	rm -f coffeeShack
