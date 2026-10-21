#include "Logger.hpp"
#include <iostream>

void firstFunction() {
   Logger::getInstance().report("firstFunction: reporting to the shared logger.");
}

void secondFunction() {
   Logger::getInstance().report("secondFunction: reporting to the shared logger.");
}

int main() {
   firstFunction();
   secondFunction();
   std::cout << "Two messages appended to log.txt.\n";
   return 0;
}
