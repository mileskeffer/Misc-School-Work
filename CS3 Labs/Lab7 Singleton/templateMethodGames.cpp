// Games, Template Method example
// Mikhail Nesterenko
// 2/4/2014

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>

using std::cout; using std::endl; using std::string; using std::cin;

// template for any game where players take 
// turns to make moves
// and there is a winner
class Game{
public:
   Game():playersCount_(0), movesCount_(0), playerWon_(noWinner){}

   // template method
   void playGame(const int playersCount = 0) {
      playersCount_ = playersCount;
      movesCount_=0;

      initializeGame();

      for(int i=0; !endOfGame(); i = (i+1) % playersCount_ ){
	 makeMove(i);
	 if (i==playersCount_-1) 
	    ++movesCount_; 
      }
      printWinner();
   }

   virtual ~Game(){}

protected:
   // primitive operations
   virtual void initializeGame() = 0;
   virtual void makeMove(int player) = 0;
   virtual void printWinner() = 0;
   virtual bool endOfGame() { return playerWon_ != noWinner;} // this is a hook
                   // returns true if winner is decided
   static const int noWinner=-1;

   int playersCount_;
   int movesCount_;
   int playerWon_;
};

// Monopoly - a concrete game implementing primitive 
// operations for the template method
class Monopoly: public Game {
public:  
   // implementing concrete methods
   void initializeGame(){
      playersCount_ = rand() % numPlayers_ + 1 ; // initialize players
   }

   void makeMove(int player) {
      if (movesCount_ > minMoves_){ 
	 const int chance = minMoves_ + rand() % (maxMoves_ - minMoves_);
	 if (chance < movesCount_) playerWon_= player;
      }
   }

   void printWinner(){
      cout << "Monopoly, player "<< playerWon_<< " won in "
	   << movesCount_<< " moves." << endl;
   }

private:
   static const int numPlayers_ = 8; // max number of players
   static const int minMoves_ = 20; // nobody wins before minMoves_
   static const int maxMoves_ = 200; // somebody wins before maxMoves_
};

// Chess - another game implementing
// primitive operations
class Chess: public Game {
public:
   void initializeGame(){
      playersCount_ = numPlayers_; // initalize players
      for(int i=0; i < numPlayers_; ++i) 
	     experience_[i] = rand() % maxExperience_ + 1 ; 
   }

   void makeMove(int player){
      if (movesCount_ > minMoves_){
	 const int chance = (rand() % maxMoves_) / experience_[player];
	 if (chance < movesCount_) playerWon_= player;
      }
   }

   void printWinner(){
      cout << "Chess, player " << playerWon_ 
	   << " with experience " << experience_[playerWon_]
	   << " won in "<< movesCount_ << " moves over"
	   << " player with experience " << experience_[playerWon_== 0 ? 1:0] 
           << endl;
   }

private:
   static const int numPlayers_ = 2;
   static const int minMoves_ = 5; // nobody wins before minMoves_
   static const int maxMoves_ = 100; // somebody wins before maxMoves_
   static const int maxExperience_ = 5; // player's experience
                              // the higher, the greater probability of winning
   int experience_[numPlayers_]; 
};

class RockPaperScissors: public Game {
protected:
   void initializeGame() override {
      playersCount_ = 2;
      playerWon_ = noWinner;
      inputEnded_ = false;
   }

   void makeMove(int player) override {
      if (player == 0) {
         // player 0 is the computer, player 1 is the human.
         const string choices[] = {"r", "p", "s"};
         computerChoice_ = choices[rand() % 3];
         return;
      }

      string humanChoice;
      while (true) {
         cout << "Human, (r)ock, (p)aper, or (s)cissors? ";
         if (!(cin >> humanChoice)) {
            inputEnded_ = true;
            return;
         }
         if (humanChoice == "R") humanChoice = "r";
         if (humanChoice == "P") humanChoice = "p";
         if (humanChoice == "S") humanChoice = "s";

         if (humanChoice == "r" || humanChoice == "p" || humanChoice == "s")
            break;
         cout << "Please enter r, p, or s." << endl;
      }

      cout << "Computer selected ";
      if (computerChoice_ == "r") cout << "rock. ";
      else if (computerChoice_ == "p") cout << "paper. ";
      else cout << "scissors. ";
      if (humanChoice == computerChoice_) {
         cout << "It is a draw." << endl << endl;
         // leave playerWon_ unchanged so the template starts another round
      } else if ((humanChoice == "r" && computerChoice_ == "s") ||
                 (humanChoice == "p" && computerChoice_ == "r") ||
                 (humanChoice == "s" && computerChoice_ == "p")) {
         playerWon_ = 1; // human wins
      } else {
         playerWon_ = 0; // computer wins
      }
   }

   bool endOfGame() override {
      return inputEnded_ || playerWon_ != noWinner;
   }

   void printWinner() override {
      if (inputEnded_) cout << "\nGame ended: input closed." << endl;
      else if (playerWon_ == 1) cout << "You won!" << endl;
      else cout << "Computer won!" << endl;
   }

private:
   std::string computerChoice_;
   bool inputEnded_ = false;
};

int main() {
   srand(time(nullptr));

   Game* gp = nullptr;

   // play chess 7 times
   for (int i = 0; i < 7; ++i){ 
      gp = new Chess;
      gp -> playGame(); 
      delete gp;
   }
      

   // play monopoly 7 times
   for (int i = 0; i < 7; ++i){
      gp = new Monopoly;
      gp -> playGame(); 
      delete gp;
   }

   RockPaperScissors rockPaperScissors;
   rockPaperScissors.playGame();
   return 0;
}
