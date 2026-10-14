// selecting oranges
// converting vectors to multimaps
// Mikhail Nesterenko
// 9/26/2022


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <string>

using namespace std;

enum class Variety {orange, pear, apple};
const string colors[] = {"red", "green", "yellow"};


int main(){
   srand(time(nullptr));
   multimap<Variety, string> tree;
   const int treeSize = rand() % 100 + 1;

   // random fruit and color selection
   for(int i = 0; i < treeSize; ++i)
      tree.emplace(static_cast<Variety>(rand() % 3), colors[rand() % 3]);


   // printing colors of oranges
   cout << "Colors of the oranges: ";
   const auto firstOrange = tree.lower_bound(Variety::orange);
   const auto afterLastOrange = tree.upper_bound(Variety::orange);
   for(auto fruit = firstOrange; fruit != afterLastOrange; ++fruit)
      cout << fruit->second << ", ";
   cout << endl;

}

