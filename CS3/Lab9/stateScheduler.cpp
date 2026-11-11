// State Scheduler
// Miles Keffer

#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>

using std::cout; using std::endl; using std::queue;

class Process;

// Invalid transitions do nothing unless the current state overrides them.
class State{
public:
   virtual void dispatch(Process *){}
   virtual void suspend(Process *){}
   virtual void block(Process *){}
   virtual void unblock(Process *){}
   virtual void report() const = 0;
   virtual ~State(){}
};

class Ready: public State{
public:
   void dispatch(Process *process) override;
   void report() const override {cout << "ready";}
};

class Running: public State{
public:
   void suspend(Process *process) override;
   void block(Process *process) override;
   void report() const override {cout << "running";}
};

class Blocked: public State{
public:
   void unblock(Process *process) override;
   void report() const override {cout << "blocked";}
};

// Context: each process has its own three state objects.
class Process{
public:
   Process(): id_(nextId_++), state_(&ready_){}
   int getId() const {return id_;}

   void dispatch(){state_->dispatch(this);}
   void suspend(){state_->suspend(this);}
   void block(){state_->block(this);}
   void unblock(){state_->unblock(this);}

   void changeState(State *state){state_ = state;}
   State *readyState(){return &ready_;}
   State *runningState(){return &running_;}
   State *blockedState(){return &blocked_;}

   void report() const{
      cout << "process " << id_ << " is ";
      state_->report();
      cout << endl;
   }
private:
   static int nextId_;
   int id_;
   Ready ready_;
   Running running_;
   Blocked blocked_;
   State *state_;
};

int Process::nextId_ = 1;

void Ready::dispatch(Process *process){
   process->changeState(process->runningState());
}

void Running::suspend(Process *process){
   process->changeState(process->readyState());
}

void Running::block(Process *process){
   process->changeState(process->blockedState());
}

void Blocked::unblock(Process *process){
   process->changeState(process->readyState());
}

int main(){
   std::srand(static_cast<unsigned int>(std::time(nullptr)));
   queue<Process *> readyQueue;
   queue<Process *> blockedQueue;

   const int numProcesses = 4;
   for(int i=0; i<numProcesses; ++i){
      Process *process = new Process;
      cout << "process " << process->getId() << ": created" << endl;
      process->report();
      readyQueue.push(process);
   }

   cout << endl << "---------- scheduler running -------------" << endl;
   while(!readyQueue.empty() || !blockedQueue.empty()){
      if(!readyQueue.empty()){
         Process *process = readyQueue.front();
         cout << "Dispatching..." << endl;
         process->dispatch();
         process->report();

         // Equal chances of exiting, suspension, and blocking.
         int action = std::rand() % 3;
         readyQueue.pop();
         if(action == 0){
            cout << "Exiting..." << endl;
            cout << "process " << process->getId() << ": exited" << endl;
            delete process;
         }else if(action == 1){
            cout << "Suspending..." << endl;
            process->suspend();
            process->report();
            readyQueue.push(process);
         }else{
            cout << "Blocking..." << endl;
            process->block();
            process->report();
            blockedQueue.push(process);
         }
         cout << endl;
      }else{
         cout << "The ready queue is empty." << endl;
      }

      // Give the head blocked process a 50% chance to become ready.
      // This also runs when all remaining processes are blocked.
      if(!blockedQueue.empty() && std::rand() % 2 == 0){
         Process *process = blockedQueue.front();
         blockedQueue.pop();
         cout << "Unblocking..." << endl;
         process->unblock();
         process->report();
         readyQueue.push(process);
         cout << endl;
      }
   }

   cout << "All processes exited." << endl;
   cout << "---------- scheduler done -------------" << endl;
   return 0;
}
