// needed for lab
// Mikhail Nesterenko
// 3/18/2022
// modified by Miles Keffer
// 10/28/2026

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "carFactory.hpp"

using std::vector; using std::string;
using std::cout; using std::endl;

class CarLot{
public:
   CarLot();

   // moves to the next car on the lot (wrapping around) and returns it
   Car *nextCar(){
      current_ = (current_ + 1) % lot_.size();
      return lot_[current_];
   }

   int lotSize() const {return lot_.size();}

   // buys the car last returned by nextCar(),
   // requests a new one from a random factory to replace it
   Car *buyCar(){
      Car *bought = lot_[current_];
      lot_[current_] =
        factories_[rand()%factories_.size()]->requestCar();
      return bought;
   }

private:
   static const int maxLotSize_ = 8; // lot holds up to this many cars
   vector<Car *> lot_;               // cars for sale at the lot
   size_t current_;                  // car currently being test-driven
   vector<CarFactory *> factories_;
};


CarLot::CarLot(){
   // creates 2 Ford factories and 2 Toyota factories
   factories_.push_back(new FordFactory());
   factories_.push_back(new ToyotaFactory());
   factories_.push_back(new FordFactory());
   factories_.push_back(new ToyotaFactory());

   // fills the lot with random cars
   for(int i=0; i < maxLotSize_; ++i)
      lot_.push_back(factories_[rand() % factories_.size()] -> requestCar());

   // first nextCar() returns the first car on the lot
   current_ = lot_.size() - 1;
}



CarLot *carLotPtr = nullptr; // global pointer instantiation

// test-drives up to lotSize() cars
// buys the first one of the wanted make and model
void shopFor(const string &buyer, const string &make, const string &model){
   if (carLotPtr == nullptr)
      carLotPtr = new CarLot();

   cout << buyer << " wants a " << make << " " << model << endl;

   for(int i=0; i < carLotPtr -> lotSize(); ++i){
      Car *toBuy = carLotPtr -> nextCar();

      cout << " test driving "
	   << toBuy->getMake() << " "
	   << toBuy->getModel();

      if (toBuy->getMake() == make && toBuy->getModel() == model){
	 cout << " love it! buying it!" << endl;
	 delete carLotPtr -> buyCar();
	 return;
      } else
	 cout << " did not like it!" << endl;
   }
   cout << " no " << make << " " << model << " on the lot, going away" << endl;
}

// wants a Toyota of a random model
void toyotaLover(int id){
   shopFor("Jill Toyoter " + std::to_string(id), "Toyota", Toyota::randomModel());
}

// wants a Ford of a random model
void fordLover(int id){
   shopFor("Jack Fordman " + std::to_string(id), "Ford", Ford::randomModel());
}



int main() {
   srand(time(nullptr));

   const int numBuyers=20;
   for(int i=0; i < numBuyers; ++i){
      if(rand()% 2 == 0)
	 toyotaLover(i);
      else
	 fordLover(i);
      cout << endl;
   }
}
