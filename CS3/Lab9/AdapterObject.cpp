// figures: object adapter pattern
// Mikhail Nesterenko
// 11/7/2022

#include <iostream>

using std::cout; using std::endl;

// implementer/adaptee
class LegacyRectangle{
public:
   LegacyRectangle(int topLeftX, int topLeftY, 
		   int bottomRightX, int bottomRightY): 
      topLeftX_(topLeftX), 
      topLeftY_(topLeftY), 
      bottomRightX_(bottomRightX), 
      bottomRightY_(bottomRightY){}

   void oldDraw() const{
      for(int i=0; i < bottomRightY_; ++i){
         for(int j=0; j < bottomRightX_; ++j)
            if( i >= topLeftY_ && j>= topLeftX_ )
               cout << '*';
            else
               cout << ' ';
         cout << endl;
      }
   }
private: // defining top/left and bottom/right coordinates 
   int topLeftX_;
   int topLeftY_;
   int bottomRightX_;
   int bottomRightY_;
};

// target interface
class Square{
public:
   virtual void draw() const =0;
   virtual ~Square(){}
};



// adapter uses composition to delegate 
// requests to adaptee
class SquareAdapter: public Square{
public:
   SquareAdapter(int size){
      adaptee_= new LegacyRectangle(0,0,size,size);
   }
   void draw() const override {
      adaptee_->oldDraw();
   }
private:
   LegacyRectangle *adaptee_;
};


int main(){
   Square *square = new SquareAdapter(6);
   square->draw();
}
