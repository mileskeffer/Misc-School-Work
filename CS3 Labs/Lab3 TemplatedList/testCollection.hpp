// Templated singly linked-list collection

#ifndef TESTCOLLECTION_HPP_
#define TESTCOLLECTION_HPP_

#include "list.hpp"

template <typename T>
class Collection {
public:
   void add(const T& item);

private:
   node<T>* head_ = nullptr;
};

//
// Collection member function definitions
//

template <typename T>
void Collection<T>::add(const T& item) {
   node<T>* const newNode = new node<T>;
   newNode->setData(item);

   if (head_ == nullptr) {
      head_ = newNode;
      return;
   }

   node<T>* current = head_;
   while (current->getNext() != nullptr)
      current = current->getNext();

   current->setNext(newNode);
}

#endif // TESTCOLLECTION_HPP_
