#include "WordList.hpp"

#include <algorithm>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

WordOccurrence::WordOccurrence(const string& word, int num)
   : word_(word), num_(num) {}

bool WordOccurrence::matchWord(const string& word) {
   return word_ == word;
}

void WordOccurrence::increment() {
   ++num_;
}

string WordOccurrence::getWord() const {
   return word_;
}

int WordOccurrence::getNum() const {
   return num_;
}

WordList::WordList()
   : wordArray_(nullptr), size_(0) {}

WordList::WordList(const WordList& other)
   : wordArray_(nullptr), size_(other.size_) {
   if (size_ > 0) {
      wordArray_ = new WordOccurrence[size_];
      for (int i = 0; i < size_; ++i) {
         wordArray_[i] = other.wordArray_[i];
      }
   }
}

WordList::~WordList() {
   delete[] wordArray_;
}

WordList& WordList::operator=(const WordList& other) {
   if (this != &other) {
      WordOccurrence* newArray = nullptr;

      if (other.size_ > 0) {
         newArray = new WordOccurrence[other.size_];
         for (int i = 0; i < other.size_; ++i) {
            newArray[i] = other.wordArray_[i];
         }
      }

      delete[] wordArray_;
      wordArray_ = newArray;
      size_ = other.size_;
   }

   return *this;
}

void WordList::addWord(const string& word) {
   for (int i = 0; i < size_; ++i) {
      if (wordArray_[i].matchWord(word)) {
         wordArray_[i].increment();
         return;
      }
   }

   WordOccurrence* newArray = new WordOccurrence[size_ + 1];
   for (int i = 0; i < size_; ++i) {
      newArray[i] = wordArray_[i];
   }
   newArray[size_] = WordOccurrence(word, 1);

   delete[] wordArray_;
   wordArray_ = newArray;
   ++size_;
}

void WordList::print() {
   std::sort(wordArray_, wordArray_ + size_,
      [](const WordOccurrence& left, const WordOccurrence& right) {
         if (left.getNum() != right.getNum()) {
            return left.getNum() < right.getNum();
         }
         return left.getWord() < right.getWord();
      });

   for (int i = 0; i < size_; ++i) {
      cout << wordArray_[i].getWord() << ' ' << wordArray_[i].getNum() << endl;
   }
}

bool equal(const WordList& first, const WordList& second) {
   if (first.size_ != second.size_) {
      return false;
   }

   for (int i = 0; i < first.size_; ++i) {
      bool foundMatch = false;

      for (int j = 0; j < second.size_; ++j) {
         if (first.wordArray_[i].getWord() == second.wordArray_[j].getWord() &&
             first.wordArray_[i].getNum() == second.wordArray_[j].getNum()) {
            foundMatch = true;
            break;
         }
      }

      if (!foundMatch) {
         return false;
      }
   }

   return true;
}
