#include "WordList.hpp"

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

int main(int argc, char* argv[]) {
   if (argc != 2) {
      cerr << "Usage: " << argv[0] << " filename" << endl;
      return 1;
   }

   ifstream input(argv[1]);
   if (!input) {
      cerr << "Cannot open " << argv[1] << endl;
      return 1;
   }

   WordList words;
   string currentWord;
   char ch;

   while (input.get(ch)) {
      unsigned char value = static_cast<unsigned char>(ch);

      if (std::isalnum(value)) {
         currentWord += static_cast<char>(std::tolower(value));
      } else if (!currentWord.empty()) {
         words.addWord(currentWord);
         currentWord.clear();
      }
   }

   if (!currentWord.empty()) {
      words.addWord(currentWord);
   }

   words.print();

   return 0;
}
