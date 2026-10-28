// filled/hollow figures demonstrates Bridge Design Pattern,
// square is either hollow or square and is painted with a particular character,
// it is bridged over Figure --> Fill abstract body/handle
// Mikhail Nesterenko
// 2/12/2014
// modified by Miles Keffer
// 10/28/2026

#include <iostream>
#include <cstdlib>
#include <ctime>

using std::cout; using std::endl;

// abstract body
class Fill{
public:
   Fill(char borderChar, char internalChar):
      borderChar_(borderChar), internalChar_(internalChar){}
   virtual char getBorder()=0;
   virtual char getInternal()=0;
   virtual ~Fill() {}
protected:
   char borderChar_;   // border paint
   char internalChar_; // internal paint
};

// concrete body
class Hollow: public Fill{
public:
   Hollow(char borderChar):Fill(borderChar, ' '){}
   char getBorder() override {return borderChar_;}
   char getInternal() override {return internalChar_;}
   ~Hollow() override {}
};


// another concrete body
class Filled: public Fill {
public:
   Filled(char fillChar):Fill(fillChar, fillChar){}
   char getBorder() override {return borderChar_;}
   char getInternal() override {return internalChar_;}
   ~Filled() override {}
protected:
   // lets derived classes use different border and internal paint
   Filled(char borderChar, char internalChar):Fill(borderChar, internalChar){}
};

// filled, but border and inside painted differently
class FullyFilled: public Filled {
public:
   FullyFilled(char borderChar, char internalChar):
      Filled(borderChar, internalChar){}
   ~FullyFilled() override {}
};

// each paint request randomly returns one of the two characters
class RandomFilled: public Fill {
public:
   RandomFilled(char firstChar, char secondChar):Fill(firstChar, secondChar){}
   char getBorder() override {return randomChar();}
   char getInternal() override {return randomChar();}
   ~RandomFilled() override {}
private:
   char randomChar() const {return rand() % 2 == 0 ? borderChar_ : internalChar_;}
};

// abstract handle
class Figure {
public:
   Figure(int size, Fill* fill): size_(size), fill_(fill){}
   virtual void draw() =0;
   virtual ~Figure(){}
protected:
   int size_;
   Fill *fill_;
};

// concrete handle
class Square: public Figure{
public:
   Square(int size, Fill* fill): Figure(size, fill){}
   void draw() override;

};

void Square::draw(){
   for(int i=0; i < size_; ++i){
      for(int j=0; j < size_; ++j)
	 if(i==0 || j==0 || i==size_-1 || j==size_-1 )
	    cout << fill_ -> getBorder();
	 else
	    cout << fill_ -> getInternal();
      cout << endl;
   }
}

int main(){
   srand(time(nullptr));

   Fill* hollowPaintStar = new Hollow('*');
   Fill* filledPaintJ = new Filled('j');
   Fill* fullyFilledPaint = new FullyFilled('#', 'o');
   Fill* randomFilledPaint = new RandomFilled('x', '.');

   Figure *hollowBox = new Square(10, hollowPaintStar);
   Figure *filledBox = new Square(10, filledPaintJ);
   Figure *fullyFilledBox = new Square(10, fullyFilledPaint);
   Figure *randomFilledBox = new Square(10, randomFilledPaint);

   cout << "Hollow('*'):" << endl;
   hollowBox -> draw();
   cout << endl;

   cout << "Filled('j'):" << endl;
   filledBox -> draw();
   cout << endl;

   cout << "FullyFilled('#', 'o'):" << endl;
   fullyFilledBox -> draw();
   cout << endl;

   cout << "RandomFilled('x', '.'):" << endl;
   randomFilledBox -> draw();
   cout << endl;

   delete hollowBox; delete filledBox; delete fullyFilledBox; delete randomFilledBox;
   delete hollowPaintStar; delete filledPaintJ;
   delete fullyFilledPaint; delete randomFilledPaint;
}
