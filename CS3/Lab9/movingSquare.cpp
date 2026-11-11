// Adapter Moving Square
// Miles Keffer

#include <iostream>

using std::cin; using std::cout; using std::endl;

// Adaptee: a rectangle described by its corner coordinates.
class LegacyRectangle{
public:
   LegacyRectangle(int topLeftX, int topLeftY,
                   int bottomRightX, int bottomRightY):
      topLeftX_(topLeftX), topLeftY_(topLeftY),
      bottomRightX_(bottomRightX), bottomRightY_(bottomRightY){}

   void move(int topLeftX, int topLeftY,
             int bottomRightX, int bottomRightY){
      topLeftX_ = topLeftX;
      topLeftY_ = topLeftY;
      bottomRightX_ = bottomRightX;
      bottomRightY_ = bottomRightY;
   }

   int getTopLeftX() const {return topLeftX_;}
   int getTopLeftY() const {return topLeftY_;}
   int getBottomRightX() const {return bottomRightX_;}
   int getBottomRightY() const {return bottomRightY_;}

   void oldDraw() const{
      for(int i=0; i<bottomRightY_; ++i){
         for(int j=0; j<bottomRightX_; ++j){
            if(i >= topLeftY_ && j >= topLeftX_)
               cout << '*';
            else
               cout << ' ';
         }
         cout << endl;
      }
   }
private:
   int topLeftX_;
   int topLeftY_;
   int bottomRightX_;
   int bottomRightY_;
};

// Target interface.
class Square{
public:
   virtual void draw() const = 0;
   virtual int size() const = 0;
   virtual void resize(int newSize) = 0;
   virtual ~Square(){}
};

// Object adapter: translate square operations into rectangle operations.
class SquareAdapter: public Square{
public:
   SquareAdapter(int size): adaptee_(0, 0, size, size){}

   void draw() const override {adaptee_.oldDraw();}

   int size() const override{
      return adaptee_.getBottomRightX() - adaptee_.getTopLeftX();
   }

   void resize(int newSize) override{
      if(newSize <= 0)
         return;
      int x = adaptee_.getTopLeftX();
      int y = adaptee_.getTopLeftY();
      adaptee_.move(x, y, x + newSize, y + newSize);
   }
private:
   LegacyRectangle adaptee_;
};

int main(){
   int size;
   cout << "Enter the size of a square: ";
   if(!(cin >> size) || size <= 0){
      cout << "The size must be a positive integer." << endl;
      return 1;
   }

   SquareAdapter square(size);
   cout << "Square size: " << square.size() << endl;
   square.draw();

   cout << "Enter the new size: ";
   if(!(cin >> size) || size <= 0){
      cout << "The size must be a positive integer." << endl;
      return 1;
   }

   square.resize(size);
   cout << "Square size: " << square.size() << endl;
   square.draw();
   return 0;
}
