// jamming peaches
// using STL algorithms to process peaches
// Mikhail Nesterenko
// 10/7/2025


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>
#include <deque>
#include <string>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;

struct Peaches{
   double weight; // oz
   bool ripe;  // ripe or not
   void print() const { cout << (ripe?"ripe":"green") << ", " <<  weight << endl; }
};

bool lighterThan(const Peaches& peach, double maxWeight) {
   return peach.weight < maxWeight;
}


int main(){
   srand(time(nullptr));
   const double minWeight = 3.;
   const double maxWeight = 8.;

   cout << "Input basket size: ";
   int size;
   cin >> size;

   vector <Peaches> basket(size);

   // assign random weight and ripeness to peaches in the basket
   std::generate(basket.begin(), basket.end(), [=]() {
      return Peaches{minWeight + static_cast<double>(rand()) / RAND_MAX *
                     (maxWeight - minWeight),
                     static_cast<bool>(rand() % 2)};
   });

   
   cout << "all peaches"<< endl;
   std::for_each(basket.cbegin(), basket.cend(),
                 [](const Peaches& peach) { peach.print(); });

   deque<Peaches> peck;
   std::remove_copy_if(basket.cbegin(), basket.cend(),
                       std::front_inserter(peck),
                       [](const Peaches& peach) { return !peach.ripe; });
   basket.erase(std::remove_if(basket.begin(), basket.end(),
                               [](const Peaches& peach) {
                                  return peach.ripe;
                               }),
                basket.end());

   cout << "peaches remaining in the basket"<< endl;
   std::for_each(basket.cbegin(), basket.cend(),
                 [](const Peaches& peach) { peach.print(); });

   cout << endl;

   cout << "peaches moved to the peck"<< endl;
   std::for_each(peck.cbegin(), peck.cend(),
                 [](const Peaches& peach) { peach.print(); });


   // prints every "space" peach in the peck
   const int space=3; 
   cout << "\nevery " << space << "\'d peach in the peck"<< endl;

   auto peach = peck.cbegin();
   while (std::distance(peach, peck.cend()) >= space) {
      std::advance(peach, space - 1);
      peach->print();
      std::advance(peach, 1);
   }


   const double weightToJam = 10.0;
   using std::placeholders::_1;
   const auto shouldJam = std::bind(lighterThan, _1, weightToJam);

   const double jamWeight = std::accumulate(
      peck.cbegin(), peck.cend(), 0.0,
      [&shouldJam](double total, const Peaches& current) {
         return total + (shouldJam(current) ? current.weight : 0.0);
      });

   peck.erase(std::remove_if(peck.begin(), peck.end(), shouldJam), peck.end());

   cout << "Weight of jam is: " << jamWeight << endl;

}

