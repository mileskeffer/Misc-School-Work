// private inheritance, 
// brings feature into interface of derived class with "using"
// Mikhail Nesterenko
// 3/26/2026

#include <iostream>

using std::cout; using std::endl;

// base class
class Figure {
public:
   Figure(int size): size_(size){}
   int findCenter() const {return size_/2;}
protected:
   int size_;
};

// derived class
class Square: private Figure{
public:
   Square(int size): Figure(size){}
   void draw(); 
   using Figure::findCenter; 
};

/*
class Square: public Figure{
public:
   Square(int size): Figure(size){}
   void draw();
private: // can also make a public method private
     using Figure::findCenter;
};
*/

void Square::draw(){
   for(int i=0; i < size_; ++i){
      for(int j=0; j < size_; ++j)
	 cout << '*';
      cout << endl;
   }
}


int main(){
   Square small(3), big(27);
   small.draw();
   cout << "Center of the small square is: " 
	<< small.findCenter() << endl;

   big.draw();
   cout << "Center of the big square is: " 
	<< big.findCenter() << endl;
}