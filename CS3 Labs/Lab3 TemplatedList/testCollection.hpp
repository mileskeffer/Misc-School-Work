// Templated singly linked-list collection

#ifndef TESTCOLLECTION_HPP_
#define TESTCOLLECTION_HPP_

#include <iostream>
#include <stdexcept>
#include <utility>

#include "list.hpp"

template <typename T>
class Collection;

template <typename T>
bool equal(const Collection<T>&, const Collection<T>&);

template <typename T>
class Collection {
public:
   Collection();
   Collection(const Collection<T>&);
   ~Collection();

   Collection<T>& operator=(const Collection<T>&);

   void add(const T& item);
   void remove(const T& item);
   T last() const;
   void print() const;

private:
   node<T>* head_ = nullptr;

   friend bool equal<T>(const Collection<T>&, const Collection<T>&);
};

//
// Collection member function definitions
//

template <typename T>
Collection<T>::Collection() : head_(nullptr) {}

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

template <typename T>
void Collection<T>::remove(const T& item) {
   while (head_ != nullptr && head_->getData() == item) {
      node<T>* const oldHead = head_;
      head_ = head_->getNext();
      delete oldHead;
   }

   if (head_ == nullptr)
      return;

   node<T>* current = head_;
   while (current->getNext() != nullptr) {
      if (current->getNext()->getData() == item) {
         node<T>* const removed = current->getNext();
         current->setNext(removed->getNext());
         delete removed;
      } else {
         current = current->getNext();
      }
   }
}

template <typename T>
T Collection<T>::last() const {
   if (head_ == nullptr)
      throw std::out_of_range("cannot get the last item of an empty collection");

   node<T>* current = head_;
   while (current->getNext() != nullptr)
      current = current->getNext();

   return current->getData();
}

template <typename T>
void Collection<T>::print() const {
   for (node<T>* current = head_; current != nullptr;
        current = current->getNext())
      std::cout << current->getData() << ' ';

   std::cout << std::endl;
}

template <typename T>
Collection<T>::Collection(const Collection<T>& other) {
   for (node<T>* current = other.head_; current != nullptr;
        current = current->getNext())
      add(current->getData());
}

template <typename T>
Collection<T>::~Collection() {
   while (head_ != nullptr) {
      node<T>* const removed = head_;
      head_ = head_->getNext();
      delete removed;
   }
}

template <typename T>
Collection<T>& Collection<T>::operator=(const Collection<T>& rhs) {
   if (this != &rhs) {
      Collection<T> copy(rhs);
      std::swap(head_, copy.head_);
   }

   return *this;
}

template <typename T>
bool equal(const Collection<T>& lhs, const Collection<T>& rhs) {
   node<T>* left = lhs.head_;
   node<T>* right = rhs.head_;

   while (left != nullptr && right != nullptr) {
      if (!(left->getData() == right->getData()))
         return false;

      left = left->getNext();
      right = right->getNext();
   }

   return left == nullptr && right == nullptr;
}

#endif // TESTCOLLECTION_HPP_
