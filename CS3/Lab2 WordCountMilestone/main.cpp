#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::isalnum;
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

   string word;
   char ch;

   while (input.get(ch)) {
      unsigned char value = static_cast<unsigned char>(ch);

      if (isalnum(value)) {
         word += ch;
      } else {
         if (!word.empty()) {
            cout << word << endl;
            word.clear();
         }
      }
   }

   if (!word.empty()) {
      cout << word << endl;
   }

   return 0;
}
