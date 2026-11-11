// figures: class adapter pattern
// Mikhail Nesterenko
// 11/7/2022

#include <iostream>

using std::cout; using std::endl;

// adaptee/implementer
class LegacyRectangle{
public:
   LegacyRectangle(int topLeftX, 
		   int topLeftY, 
		   int bottomRightX, 
		   int bottomRightY): 
      topLeftX_(topLeftX), 
      topLeftY_(topLeftY), 
      bottomRightX_(bottomRightX), 
      bottomRightY_(bottomRightY){}

   void oldDraw() const {
      for(int i=0; i < bottomRightY_; ++i){
	 for(int j=0; j < bottomRightX_; ++j)
	    if(i >= topLeftY_  && j >= topLeftX_ )
	       cout << '*';
	    else
	       cout << ' ';
	 cout << endl;
      }
   }
// defining top/left and bottom/right coordinates 
private: 
   const int topLeftX_;
   const int topLeftY_;
   const int bottomRightX_;
   const int bottomRightY_;
};

// target interface
class Square {
public:
   virtual void draw() const =0;
   virtual ~Square(){}
};


// adapter uses multiple inheritance to inherit
// interface and implementation
class SquareAdapter: public Square, 
		     private LegacyRectangle {
public:
   SquareAdapter(int size): LegacyRectangle(0,0,size,size){};
   void draw() const override {
      oldDraw();
   }
};


int main(){
   Square *square = new SquareAdapter(16);
   square->draw();
}