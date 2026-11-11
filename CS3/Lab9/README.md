Lab 9 - Adapter Moving Square and State Scheduler
Miles Keffer

The two programs have separate main functions. Build and run them separately:

```text
g++ -std=c++11 -Wall -Wextra -pedantic movingSquare.cpp -o movingSquare
g++ -std=c++11 -Wall -Wextra -pedantic stateScheduler.cpp -o stateScheduler
```

In PowerShell:

```powershell
./movingSquare.exe
./stateScheduler.exe
```

`movingSquare.cpp` uses the object Adapter pattern from the provided example.
Enter an initial positive size and a new positive size. The program draws both
squares. The adapter calculates the side length from the rectangle coordinates
and resizes by moving the bottom right corner while keeping the top left fixed.

`stateScheduler.cpp` uses the State pattern. Each process starts ready and gets
a unique ID from a static counter. State objects handle the four transitions;
inappropriate transitions do nothing. Two STL queues hold pointers to processes
so that each process keeps its identity and is destroyed when it exits.

The scheduler admits four processes. It dispatches the front ready process,
then randomly exits it, suspends it to the back of the ready queue, or blocks it
to the back of the blocked queue (equal chances). Each iteration gives the front
blocked process a 50% chance of returning to the ready queue, including when the
ready queue is empty. Scheduling continues until all four processes have exited.
Each transition reports the affected process's state. Output varies between runs.

`square-output.txt` demonstrates resizing from 3 to 2. `scheduler-output.txt`
contains an actual run scheduling all four processes through completion.

The original class examples and `stateTransitionsExample.txt` are retained for
reference. Coffee Shack belongs to Lab 10 and is in `../Lab10 CoffeeShack/solution`.
