// Coffee Shack: Decorator, Chain of Responsibility, and Observer.
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "drink.hpp"

using std::cin; using std::cout; using std::endl;
using std::string; using std::vector;

// Each decorator owns the drink it wraps, including earlier ingredients.
class Ingredient: public Drink{
public:
   Ingredient(Drink *drink): drink_(drink){}
   ~Ingredient() override {delete drink_;}
   string getName() const override{
      string name = drink_->getName();
      if(name.find(" with ") == string::npos)
         return name + " with ";
      return name + ", ";
   }
protected:
   Drink *drink_;
};

class Sugar: public Ingredient{
public:
   Sugar(Drink *drink): Ingredient(drink){}
   int getPrice() const override {return drink_->getPrice() + 1;}
   string getName() const override {return Ingredient::getName() + "sugar";}
};

class Cream: public Ingredient{
public:
   Cream(Drink *drink): Ingredient(drink){}
   int getPrice() const override {return drink_->getPrice() + 2;}
   string getName() const override {return Ingredient::getName() + "cream";}
};

class Honey: public Ingredient{
public:
   Honey(Drink *drink): Ingredient(drink){}
   int getPrice() const override {return drink_->getPrice() + 3;}
   string getName() const override {return Ingredient::getName() + "honey";}
};

// Observer: every waiting customer receives each ready-drink announcement.
class Customer{
public:
   Customer(const string &name, Drink *drink): name_(name), drink_(drink){}
   ~Customer(){delete drink_;}
   string getName() const {return name_;}
   const Drink *getDrink() const {return drink_;}
   void update(const Customer *ready) const{
      // Compare customers, so two customers may have the same name.
      if(ready == this)
         cout << "This is " << name_ << ", I got my coffee, thank you!" << endl;
   }
private:
   string name_;
   Drink *drink_;
};

// Subject and chain handler. The next barista is not owned by this object.
class Barista{
public:
   Barista(Barista *next=nullptr): next_(next){}
   virtual ~Barista(){}
   virtual string prepare(const Drink *drink) const = 0;

   void registerCustomer(Customer *customer){
      customers_.push_back(customer);
   }

   void unregisterCustomer(Customer *customer){
      for(size_t i=0; i<customers_.size(); ++i){
         if(customers_[i] == customer){
            customers_.erase(customers_.begin() + i);
            return;
         }
      }
   }

   bool hasCustomers() const {return !customers_.empty();}

   void notifyCustomers(const Customer *ready) const{
      for(size_t i=0; i<customers_.size(); ++i)
         customers_[i]->update(ready);
   }

   void serveCustomer(){
      if(customers_.empty())
         return;

      // Choosing any waiting customer allows drinks to finish out of order.
      Customer *ready = customers_[std::rand() % customers_.size()];
      const Drink *drink = ready->getDrink();
      string level = prepare(drink);
      cout << endl << ready->getName() << ", your " << drink->getName()
           << " is ready. It is prepared by a " << level
           << ". It will be $" << drink->getPrice() << ", please." << endl;
      notifyCustomers(ready);
      unregisterCustomer(ready);
      cout << endl;
   }
protected:
   Barista *next_;
private:
   vector<Customer *> customers_;
};

class JuniorBarista: public Barista{
public:
   JuniorBarista(Barista *next): Barista(next){}
   string prepare(const Drink *drink) const override{
      // An undecorated drink has no ingredient list.
      if(drink->getName().find(" with ") == string::npos)
         return "JuniorBarista";
      return next_->prepare(drink);
   }
};

class SeniorBarista: public Barista{
public:
   SeniorBarista(Barista *next): Barista(next){}
   string prepare(const Drink *drink) const override{
      if(drink->getName().find("honey") == string::npos)
         return "SeniorBarista";
      return next_->prepare(drink);
   }
};

class Manager: public Barista{
public:
   string prepare(const Drink *) const override {return "Manager";}
};

Customer *takeOrder(){
   char choice;
   Drink *drink = nullptr;
   while(drink == nullptr){
      cout << "Welcome to Coffee Shack, can I get you [l]arge, [m]edium, or [s]mall coffee? ";
      if(!(cin >> choice))
         return nullptr;
      switch(choice){
      case 'l': drink = new Drink(DrinkType::large, 10); break;
      case 'm': drink = new Drink(DrinkType::medium, 7); break;
      case 's': drink = new Drink(DrinkType::small, 5); break;
      default: cout << "Please enter l, m, or s." << endl;
      }
   }

   do{
      cout << "Would you like to add [s]ugar, [c]ream, [h]oney, or [d]one? ";
      if(!(cin >> choice)){
         delete drink;
         return nullptr;
      }
      switch(choice){
      case 's': drink = new Sugar(drink); break;
      case 'c': drink = new Cream(drink); break;
      case 'h': drink = new Honey(drink); break;
      case 'd': break;
      default: cout << "Please enter s, c, h, or d." << endl;
      }
   }while(choice != 'd');

   string name;
   cout << "Can I get your name? ";
   if(!std::getline(cin >> std::ws, name)){
      delete drink;
      return nullptr;
   }
   cout << "Please wait, " << name << "." << endl << endl;
   return new Customer(name, drink);
}

int main(){
   std::srand(static_cast<unsigned int>(std::time(nullptr)));
   Manager manager;
   SeniorBarista senior(&manager);
   JuniorBarista junior(&senior);
   vector<Customer *> customers;

   // Demonstrate a group of three customers without using threads.
   const int numCustomers = 3;
   for(int i=0; i<numCustomers; ++i){
      Customer *customer = takeOrder();
      if(customer == nullptr)
         break;
      customers.push_back(customer);
      junior.registerCustomer(customer);

      // Otherwise, continue taking the next customer's order.
      if(std::rand() % 2 == 0)
         junior.serveCustomer();
   }

   // Finish all accepted orders when the group is complete or input ends.
   while(junior.hasCustomers())
      junior.serveCustomer();

   for(size_t i=0; i<customers.size(); ++i)
      delete customers[i];
}
