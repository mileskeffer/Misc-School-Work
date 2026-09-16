# CS3 Lab 3 Code Review Quiz

Name: ________________________________    Date: __________________

Choose the best answer for each question. Assume the Lab 3 `node<T>` and `Collection<T>` implementations are used exactly as provided.

## 1. What does `Collection<int> numbers;` demonstrate?

A. A collection that can store values of any type at runtime
B. A template instantiation whose item type is `int`
C. A node that can store only one integer permanently
D. A collection that cannot be copied

## 2. After the following statements execute, what does `items.last()` return?

```cpp
Collection<int> items;
items.add(4);
items.add(7);
items.add(2);
```

A. 4
B. 7
C. 2
D. It throws `std::out_of_range`

## 3. What happens after this call?

```cpp
Collection<int> values;
values.add(3);
values.add(1);
values.add(3);
values.remove(3);
```

A. The collection contains `3 1`
B. The collection contains `1 3`
C. The collection contains only `1`
D. The program throws because duplicate values are not allowed

## 4. What does `last()` do when called on an empty collection?

A. Returns `nullptr`
B. Returns a default-constructed `T`
C. Prints an error and returns `-1`
D. Throws `std::out_of_range`

## 5. What is printed by the following code?

```cpp
Collection<std::string> words;
words.add("red");
words.add("blue");
words.print();
```

A. `redblue` with no spaces or newline
B. `red blue ` followed by a newline
C. `blue red` followed by a newline
D. The memory addresses of the two nodes

## 6. Why does the copy constructor add each item from `other` instead of simply assigning `head_`?

A. To preserve the order while creating separate nodes
B. To make the copied collection automatically sorted
C. To prevent `last()` from working on the copy
D. To convert every item to a string

## 7. Given the following code, what is the purpose of `std::swap(head_, copy.head_)` in the assignment operator?

```cpp
Collection<int> first;
Collection<int> second;
first.add(10);
second = first;
```

A. It swaps the values of the first and last nodes in `first`
B. It gives `second` the copied list and lets the temporary clean up the old list
C. It deletes the source collection before copying it
D. It makes `first` and `second` share the same nodes

## 8. Why is this statement safe in the provided assignment operator?

```cpp
collection = collection;
```

A. The `this != &rhs` check prevents the collection from copying itself
B. `std::swap` always ignores self-assignment
C. The destructor is never called during assignment
D. Self-assignment is converted into a move operation

## 9. Which condition must be true for `equal(lhs, rhs)` to return `true`?

A. The collections contain the same values somewhere, regardless of order
B. The collections have the same first item only
C. Corresponding items compare equal and both lists end at the same time
D. The two collections have the same `head_` pointer

## 10. What is the most important reason the destructor repeatedly deletes nodes and advances `head_`?

A. To reverse the collection before the program ends
B. To release every dynamically allocated node without following deleted pointers
C. To remove only duplicate values
D. To force `print()` to output a final newline

---

## Answer Key

1. B
2. C
3. C
4. D
5. B
6. A
7. B
8. A
9. C
10. B
