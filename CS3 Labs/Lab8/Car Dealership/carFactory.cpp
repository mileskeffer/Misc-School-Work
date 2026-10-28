// implementation of carFactory
// Mikhail Nesterenko
// 7/16/2014
// modified by Miles Keffer
// 10/28/2026

#include <array>
#include <cstdlib>
#include "carFactory.hpp"


// product methods
std::string Ford::randomModel(){
   static const std::array<std::string, 4>
      models = {"Focus ST", "Mustang", "Explorer", "F-150"};
   return models[rand() % models.size()];
}

Ford::Ford(){
   make_ = "Ford";
   model_ = randomModel();
}

std::string Toyota::randomModel(){
   static const std::array<std::string, 5>
      models = {"GR Corolla", "GR Supra", "Prius", "4Runner", "GR GT"};
   return models[rand() % models.size()];
}

Toyota::Toyota(){
   make_ = "Toyota";
   model_ = randomModel();
}


// factory methods
Car* CarFactory::requestCar() {
	++numCarsInProduction_;
	return makeCar();
}

Car* FordFactory::makeCar() {
	return new Ford();
}

Car* ToyotaFactory::makeCar() {
	return new Toyota();
}
