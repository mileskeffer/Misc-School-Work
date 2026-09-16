// Roster with class schedule -- milestone (procedural version)

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

using StudentEntry = list<string>;

// Reads each student's full name from a roster file.
list<string> readRoster(const string& fileName);

// Returns a file's name without its path or extension.
string courseName(const string& fileName);

// Finds a student in the master roster by the name at the front of the entry.
vector<StudentEntry>::iterator findStudent(vector<StudentEntry>& students,
                                           const string& name);

int main(int argc, char* argv[]) {
   if (argc < 3) {
      cout << "usage: " << argv[0]
           << " list of courses, dropouts last" << endl;
      return EXIT_FAILURE;
   }

   // Every entry contains a student's name followed by enrolled courses.
   vector<StudentEntry> studentEntries;

   for (int i = 1; i < argc - 1; ++i) {
      const string course = courseName(argv[i]);
      const list<string> roster = readRoster(argv[i]);

      for (const string& name : roster) {
         auto student = findStudent(studentEntries, name);

         if (student == studentEntries.end()) {
            studentEntries.push_back(StudentEntry{name});
            student = studentEntries.end() - 1;
         }

         // Do not list a course twice if its roster contains a duplicate name.
         auto enrolledCourse = student->begin();
         ++enrolledCourse; // Skip the student's name.
         if (std::find(enrolledCourse, student->end(), course) ==
             student->end()) {
            student->push_back(course);
         }
      }
   }

   const list<string> dropouts = readRoster(argv[argc - 1]);
   studentEntries.erase(
      std::remove_if(studentEntries.begin(), studentEntries.end(),
         [&dropouts](const StudentEntry& student) {
            return std::find(dropouts.begin(), dropouts.end(), student.front()) !=
                   dropouts.end();
         }),
      studentEntries.end());

   std::sort(studentEntries.begin(), studentEntries.end(),
      [](const StudentEntry& left, const StudentEntry& right) {
         return left.front() < right.front();
      });

   cout << "all students, dropouts removed and sorted\n"
        << "first name last name: courses enrolled\n";

   for (const StudentEntry& student : studentEntries) {
      auto item = student.begin();
      cout << *item << ':';
      for (++item; item != student.end(); ++item)
         cout << *item << ' ';
      cout << '\n';
   }
}

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

string courseName(const string& fileName) {
   const string::size_type slash = fileName.find_last_of("/\\");
   const string::size_type first =
      (slash == string::npos) ? 0 : slash + 1;
   const string::size_type dot = fileName.find_last_of('.');
   const string::size_type count =
      (dot == string::npos || dot < first) ? string::npos : dot - first;

   return fileName.substr(first, count);
}

vector<StudentEntry>::iterator findStudent(vector<StudentEntry>& students,
                                           const string& name) {
   return std::find_if(students.begin(), students.end(),
      [&name](const StudentEntry& student) {
         return student.front() == name;
      });
}
