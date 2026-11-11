// drink class to be used in Coffee Shack lab
// Mikhail Nesterenko
// 11/7/2022
// Added an include guard and virtual destructor for decorated drinks.

#ifndef DRINK_HPP
#define DRINK_HPP

#include <string>

enum class DrinkType {small, medium, large};

class Drink{
public:
   Drink(DrinkType type=DrinkType::small, int price=0):
                 price_(price), type_(type){}
   virtual ~Drink(){}
   virtual int getPrice() const;
   virtual std::string getName() const;
private:
   int price_;
   DrinkType type_;
};

#endif
