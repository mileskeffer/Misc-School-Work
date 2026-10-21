// example opening file for appending
// Mikhail Nesterenko
// 10/15/2017

#include <fstream>    

int main () {
   std::ofstream fout;
   fout.open("test.txt", std::fstream::out | std::fstream::app);
   fout << "more lorem ipsum" << std::endl;
   fout.close();
}