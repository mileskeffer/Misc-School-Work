// move sematics with an integer array object
// Mikhail Nesterenko
// 9/10/2024

#include <iostream>
#include <vector>

using std::cout; using std::endl;
using std::vector;

class MovableInt{
public:
   // regular and default constructor
   MovableInt(int size=0){
      size_= size;
      array_= new int[size];
   }

   // copy constructor
   MovableInt(const MovableInt& org){
      size_=org.size_;
      array_ = new int[size_];
      for(int i=0; i < size_; ++i) array_[i] = org.array_[i];
   }

   // move constructor 
   MovableInt(MovableInt &&org) noexcept {
      size_ = org.size_;
      org.size_ = 0;
      array_  = org.array_;
      org.array_ = nullptr;
   }

   // destructor
   ~MovableInt(){ delete[] array_;}
 

   // classic copy assignment
   // (copy-n-swap idiom is incompatbile with move assingment)
   MovableInt& operator=(const MovableInt& rhs){
      if (this != &rhs){ // protection against self-assingment
	 delete [] array_;
	 size_ = rhs.size_;
	 array_ = new int[size_];
	 for(int i=0; i < size_; ++i) array_[i] = rhs.array_[i];
      }
      return *this;
   }

   
   // move assignment
   MovableInt& operator=(MovableInt&& rhs){
      if(this != &rhs) { 
	 delete [] array_;
	 size_ = rhs.size_;
	 rhs.size_ = 0;
	 array_ = rhs.array_; // note no array copying, just pointer assingment
	 rhs.array_ = nullptr;
      }
      return *this;
   }
   
  
   // ordinary member functions to make the example interesting
   void printArray() const {
      for(int i=0; i < size_; ++i) cout << array_[i] << ' '; 
      cout << endl;
   }

   void assignArray(int n) {
      for(int i=0; i < size_; ++i) array_[i] = n;
   }

private:
  int *array_;
  int size_;
};


int main(){
   MovableInt ob1(6), ob2;
   ob1.assignArray(45);

   cout << "ob1 at the start of vector operations: "; ob1.printArray();
  
   vector<MovableInt> vmi;

   vmi.push_back(ob1);
   cout <<"ob1 after copy push_back(): "; ob1.printArray();

   vmi.push_back(std::move(ob1));
   cout << "ob1 after move push_back(): "; ob1.printArray();

   
   ob2 = vmi.back();
   cout << "ob2 after copy assingment: "; ob2.printArray();

   ob1 = std::move(ob2);

   
   // ob1 = ob2;
   cout << "ob1 afer move assingment: "; ob1.printArray();
   cout << "ob2 after move assingment: "; ob2.printArray();

}