#ifndef LAB7_LOGGER_HPP
#define LAB7_LOGGER_HPP

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>

using std::ios; using std::cerr; using std::endl; 
using std::exit; using std::ofstream; using std::string;

class Logger {
public:
   static Logger& getInstance() {
      //created only the first time getInstance() is called.
      static Logger instance;
      return instance;
   }

   void report(const string& message) {
      log_ << message << endl;
   }

   // prevent making another logger by copying the singleton.
   Logger(const Logger&) = delete;
   Logger& operator=(const Logger&) = delete;

private:
   Logger() {
      log_.open("log.txt", ios::app);
      if (!log_) {
         cerr << "Unable to open log.txt" << endl;
         exit(EXIT_FAILURE);
      }
   }

   //destroyed when the program exits.
   ~Logger() { log_.close(); }

   ofstream log_;
};

#endif
