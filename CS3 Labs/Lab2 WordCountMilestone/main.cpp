#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;

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

   int wordCount = 0;
   bool insideWord = false;
   char ch;

   while (input.get(ch)) {
      unsigned char value = static_cast<unsigned char>(ch);

      if (std::isalnum(value)) {
         if (!insideWord) {
            ++wordCount;
            insideWord = true;
         }
      } else {
         insideWord = false;
      }
   }

   cout << wordCount << endl;

   return 0;
}
