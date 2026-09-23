# CS3 Code Review - Lab 4 Roster

Name: ________________________________    Date: __________________

Choose the best answer for each question. Assume the Lab 4 roster programs are used exactly as provided.

## 1. If the program is invoked as shown below, what is the value of `argc`?

```text
classSchedule cs1.txt cs2.txt dropouts.txt
```

A. 3  
B. 4  
C. 5  
D. It depends on the number of students in the files

## 2. In the procedural version, what does one `StudentEntry` contain?

A. A course name followed by all students taking that course  
B. A student's full name followed by that student's course names  
C. Only a student's first and last name  
D. A filename followed by its file extension

## 3. What does `courseName("C:\\rosters\\cs2.txt")` return?

A. `C:\\rosters\\cs2`  
B. `cs2.txt`  
C. `cs2`  
D. `.txt`

## 4. A student named `Ava Reed` appears in both `cs1.txt` and `cs3.txt`. What will the procedural program store for her?

A. Two separate entries, one for each course  
B. One entry containing `Ava Reed`, `cs1`, and `cs3`  
C. One entry containing only the last course, `cs3`  
D. No entry, because duplicate students are removed entirely

## 5. Why does `addCourse()` call `std::find()` before `courses_.push_back(course)`?

A. To keep the courses in alphabetical order  
B. To prevent a repeated name in one input file from adding the same course twice  
C. To locate the student in `allStudents`  
D. To remove a course when a student drops out

## 6. In the object-oriented version, what happens when this statement executes for a new student?

```cpp
allStudents.push_back(std::move(student));
```

A. The `Student` move constructor transfers the strings and course list into the new list element  
B. The student is copied, and the source is guaranteed to remain unchanged  
C. The source object is destroyed before insertion begins  
D. Only the student's first name is inserted

## 7. Why does `operator==` compare only `firstName_` and `lastName_`, not `courses_`?

A. Course lists cannot be compared in C++  
B. Students with the same name must match when merging rosters or removing dropouts  
C. Comparing courses would automatically sort them  
D. `std::find` ignores the equality operator's result

## 8. According to the object-oriented `operator<`, which student appears first after `allStudents.sort()`?

A. `Zoe Adams`  
B. `Amy Young`  
C. They compare equal because only last names are checked  
D. Their order is unpredictable

## 9. After `allStudents.splice(allStudents.end(), lst)` executes in `roster.cpp`, what is true?

A. The elements are copied into `allStudents`, and `lst` keeps its elements  
B. The elements are transferred into `allStudents`, leaving `lst` empty  
C. Both lists share ownership of the same nodes  
D. `allStudents` is automatically sorted and deduplicated

## 10. Why must `allStudents.sort()` occur before `allStudents.unique()` in `roster.cpp`?

A. `unique()` removes only adjacent equal elements, so sorting brings duplicates together  
B. `unique()` works only with vectors that have first been sorted  
C. `sort()` deletes duplicates, while `unique()` restores one copy  
D. The compiler rejects a call to `unique()` on an unsorted list

---

## Answer Key

1. B  
2. B  
3. C  
4. B  
5. B  
6. A  
7. B  
8. B  
9. B  
10. A
