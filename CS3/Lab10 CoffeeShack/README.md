Coffee Shack uses the three assigned patterns with basic inheritance, pointers,
strings, and vectors. No threads or additional design patterns are used.

Build from this directory:

```text
g++ -std=c++11 -Wall -Wextra -pedantic coffeeShack.cpp drink.cpp -o coffeeShack
```

Run interactively with `./coffeeShack`. Each run accepts three customers, then
finishes all waiting orders. Ending input early also finishes accepted orders.
After each order, a random choice either serves a waiting customer or moves on
to the next order. The ready customer is chosen randomly, allowing non-FIFO
service. Concurrency here means multiple waiting orders, as in the assignment.

For redirected input in a command shell supporting `<`:

```text
coffeeShack < input.txt > output.txt
```

In PowerShell:

```powershell
Get-Content input.txt | ./coffeeShack.exe
```

`input.txt` covers all three barista levels and repeated ingredients.
`sample-output.txt` shows a run with all three customers waiting together.
Random choices make the service order vary between runs.

- Decorator: `Sugar`, `Cream`, and `Honey` wrap a `Drink`, adding to its price
  and description in the order entered.
- Chain of Responsibility: requests start at `JuniorBarista`, pass to
  `SeniorBarista` when ingredients are present, and pass to `Manager` for honey.
- Observer: customers register with the junior barista. Every waiting customer
  receives each notification and checks whether the ready order belongs to them.

The supplied `drink.hpp` was copied from the parent folder for the Lab 10
solution. Its public constructor and price/name interface are retained. An include
guard and virtual destructor were added, and the initializer order was corrected.
Deleting a decorated drink also deletes all drinks it wraps.

Honey costs $3 as specified in the assignment. Thus a large coffee with honey
costs $13, despite the $10 shown in the second screenshot example.
