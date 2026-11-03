// Roster with class schedule -- milestone (procedural version)
//
// This program is based on roster.cpp. The comments marked "Changed from
// roster.cpp" explain the changes made for the class-schedule assignment.

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::list;
using std::string;
using std::vector;

// Changed from roster.cpp: instead of storing only a name, each student is a
// list whose first item is the full name and whose remaining items are courses.
using StudentEntry = list<string>;

// Reads each student's full name from a roster file.
list<string> readRoster(const string& fileName);

// Returns a file's name without its path or extension.
string courseName(const string& fileName);

// Finds a student in the master roster by the name at the front of the entry.
vector<StudentEntry>::iterator findStudent(vector<StudentEntry>& students,
                                           const string& name);

// Prints every student entry as a name followed by enrolled courses.
void printRoster(const vector<StudentEntry>& students);

int main(int argc, char* argv[]) {
   if (argc < 3) {
      cout << "usage: " << argv[0]
           << " list of courses, dropouts last" << endl;
      return EXIT_FAILURE;
   }

   // Changed from roster.cpp: the original vector stored one separate roster
   // per course. This vector stores one combined entry per student instead.
   vector<StudentEntry> studentEntries;

   for (int i = 1; i < argc - 1; ++i) {
      // Changed from roster.cpp: save the course filename without its path or
      // extension so it can be added to each student's entry (for example cs1).
      const string course = courseName(argv[i]);
      const list<string> roster = readRoster(argv[i]);

      for (const string& name : roster) {
         // Changed from roster.cpp: search the combined roster for this student
         // rather than splicing every course roster into one list.
         auto student = findStudent(studentEntries, name);

         // If this is the student's first course, create an entry beginning
         // with the student's name.
         if (student == studentEntries.end()) {
            studentEntries.push_back(StudentEntry{name});
            student = studentEntries.end() - 1;
         }

         // Add the current course after the name. Do not list a course twice if
         // its input roster accidentally contains a duplicate name.
         auto enrolledCourse = student->begin();
         ++enrolledCourse; // Skip the student's name.
         if (std::find(enrolledCourse, student->end(), course) ==
             student->end()) {
            student->push_back(course);
         }
      }
   }

   cout << "All students\n"
        << "first name last name: courses enrolled\n";
   printRoster(studentEntries);
   cout << "\n\n";

   const list<string> dropouts = readRoster(argv[argc - 1]);

   // Changed from roster.cpp: each element is now an entire StudentEntry, so
   // remove an entry when its first item (the name) appears in dropouts.
   studentEntries.erase(
      std::remove_if(studentEntries.begin(), studentEntries.end(),
         [&dropouts](const StudentEntry& student) {
            return std::find(dropouts.begin(), dropouts.end(), student.front()) !=
                   dropouts.end();
         }),
      studentEntries.end());

   // Changed from roster.cpp: vector does not have list::sort(). Compare the
   // name at the front of each student entry with the std::sort algorithm.
   // Duplicate students were already merged above, so unique() is unnecessary.
   std::sort(studentEntries.begin(), studentEntries.end(),
      [](const StudentEntry& left, const StudentEntry& right) {
         return left.front() < right.front();
      });

   cout << "All students, dropouts removed and sorted\n"
        << "first name last name: courses enrolled\n";
   printRoster(studentEntries);
}

// Changed from roster.cpp: return the completed list instead of receiving the
// destination list by reference. File-open validation was also added.
list<string> readRoster(const string& fileName) {
   ifstream rosterFile(fileName);
   if (!rosterFile) {
      cerr << "Unable to open " << fileName << endl;
      std::exit(EXIT_FAILURE);
   }

   list<string> roster;
   string firstName;
   string lastName;
   while (rosterFile >> firstName >> lastName)
      roster.push_back(firstName + ' ' + lastName);

   return roster;
}

// New for this assignment: obtain the course label from its roster filename.
string courseName(const string& fileName) {
   const string::size_type slash = fileName.find_last_of("/\\");
   const string::size_type first =
      (slash == string::npos) ? 0 : slash + 1;
   const string::size_type dot = fileName.find_last_of('.');
   const string::size_type count =
      (dot == string::npos || dot < first) ? string::npos : dot - first;

   return fileName.substr(first, count);
}

// New for this assignment: locate a name inside the vector of student entries.
vector<StudentEntry>::iterator findStudent(vector<StudentEntry>& students,
                                           const string& name) {
   return std::find_if(students.begin(), students.end(),
      [&name](const StudentEntry& student) {
         return student.front() == name;
      });
}

// Changed from roster.cpp: print the first item as the student's name, then
// print every remaining item as an enrolled course.
void printRoster(const vector<StudentEntry>& students) {
   for (const StudentEntry& student : students) {
      auto item = student.begin();
      cout << *item << ':';
      for (++item; item != student.end(); ++item)
         cout << *item << ' ';
      cout << '\n';
   }
}
