#ifndef WORDLIST_HPP
#define WORDLIST_HPP

#include <string>

class WordOccurrence {
public:
   WordOccurrence(const std::string& word = "", int num = 0);
   bool matchWord(const std::string&);
   void increment();
   std::string getWord() const;
   int getNum() const;

private:
   std::string word_;
   int num_;
};

class WordList {
public:
   WordList();
   WordList(const WordList&);
   ~WordList();
   WordList& operator=(const WordList&);

   friend bool equal(const WordList&, const WordList&);

   void addWord(const std::string&);
   void print();

   WordOccurrence* getWords() const { return wordArray_; }
   int getSize() const { return size_; }

private:
   WordOccurrence* wordArray_;
   int size_;
};

#endif
