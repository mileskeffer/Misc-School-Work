#include "drink.hpp"

int Drink::getPrice() const{
   return price_;
}

std::string Drink::getName() const{
   if(type_ == DrinkType::large)
      return "large coffee";
   if(type_ == DrinkType::medium)
      return "medium coffee";
   return "small coffee";
}
