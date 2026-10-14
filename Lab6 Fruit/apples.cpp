// sorting apples
// using STL algorithms to process a crate of apples
// Mikhail Nesterenko
// 02/19/2025


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <string>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector;

struct Apples{
   double weight; // oz
   string color;  // red or green
   void print() const { cout << color << ", " <<  weight << endl; }
};



int main(){
   srand(time(nullptr));
   const double minWeight = 3.;
   const double maxWeight = 8.;

   cout << "Input crate size: ";
   int size;
   cin >> size;

   vector <Apples> crate(size);

   // assign random weight and color to apples in the crate
   std::generate(crate.begin(), crate.end(), [=]() {
      return Apples{minWeight + static_cast<double>(rand()) / RAND_MAX *
                    (maxWeight - minWeight),
                    rand() % 2 == 1 ? "green" : "red"};
   });


   cout << "all apples"<< endl;
   std::for_each(crate.cbegin(), crate.cend(),
                 [](const Apples& apple) { apple.print(); });

   
   cout << "Enter weight to find: ";
   double toFind;
   cin >> toFind;

   const auto cnt = std::count_if(crate.cbegin(), crate.cend(),
                                  [=](const Apples& apple) {
                                     return apple.weight > toFind;
                                  });

   cout << "There are " << cnt << " apples heavier than " 
	<< toFind << " oz" <<  endl;

   cout << "at positions ";
   auto apple = crate.cbegin();
   while ((apple = std::find_if(apple, crate.cend(),
                                [=](const Apples& current) {
                                   return current.weight > toFind;
                                })) != crate.cend()) {
      cout << std::distance(crate.cbegin(), apple) << ", ";
      ++apple;
   }
   cout << endl;


   const auto heaviest = std::max_element(
      crate.cbegin(), crate.cend(),
      [](const Apples& left, const Apples& right) {
         return left.weight < right.weight;
      });
   if (heaviest != crate.cend())
      cout << "Heaviest apple weighs: " << heaviest->weight << " oz" << endl;
   else
      cout << "The crate is empty" << endl;


   const double sum = std::accumulate(
      crate.cbegin(), crate.cend(), 0.0,
      [](double total, const Apples& apple) {
         return total + apple.weight;
      });
   cout << "Total apple weight is: " << sum << " oz" << endl;


   cout << "How much should they grow: ";
   double toGrow;
   cin >> toGrow;
   std::transform(crate.cbegin(), crate.cend(), crate.begin(),
                  [=](Apples apple) {
                     apple.weight += toGrow;
                     return apple;
                  });

   cout << "Input minimum acceptable weight: ";
   double minAccept;
   cin >> minAccept;


   // erase-remove idiom removes all apples below the acceptable weight
   crate.erase(std::remove_if(crate.begin(), crate.end(),
                              [=](const Apples& apple) {
                                 return apple.weight < minAccept;
                              }),
               crate.end());
   cout << "removed " << size - crate.size() << " elements" << endl;


   std::sort(crate.begin(), crate.end(),
             [](const Apples& left, const Apples& right) {
                return left.weight < right.weight;
             });


   cout << "sorted remaining apples"<< endl;
   std::for_each(crate.cbegin(), crate.cend(),
                 [](const Apples& apple) { apple.print(); });

}
