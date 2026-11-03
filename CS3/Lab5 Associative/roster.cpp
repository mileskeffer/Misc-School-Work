// Roster with class schedules using associative containers

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::list;
using std::map;
using std::set;
using std::string;

class Student {
public:
   Student(string firstName, string lastName)
      : firstName_(std::move(firstName)), lastName_(std::move(lastName)) {}

   string print() const {
      return firstName_ + ' ' + lastName_;
   }

   // std::map and std::set use this ordering to identify and sort students.
   friend bool operator<(const Student& left, const Student& right) {
      return left.firstName_ < right.firstName_ ||
             (left.firstName_ == right.firstName_ &&
              left.lastName_ < right.lastName_);
   }

private:
   string firstName_;
   string lastName_;
};

list<Student> readRoster(const string& fileName);
string courseName(const string& fileName);
void printSchedules(const map<Student, list<string>>& schedules);
void printStudents(const set<Student>& students);

int main(int argc, char* argv[]) {
   if (argc < 3) {
      cout << "usage: " << argv[0]
           << " list of courses, dropouts last" << endl;
      return EXIT_FAILURE;
   }

   // Each key occurs only once, while its value records every course in
   // command-line order.
   map<Student, list<string>> schedules;

   for (int i = 1; i < argc - 1; ++i) {
      const string course = courseName(argv[i]);

      for (const Student& student : readRoster(argv[i])) {
         list<string>& courses = schedules[student];

         // Protect against a duplicate name within one input roster.
         if (std::find(courses.begin(), courses.end(), course) == courses.end())
            courses.push_back(course);
      }
   }

   // A dropout is removed from the map regardless of how many courses the
   // student previously appeared in.
   for (const Student& dropout : readRoster(argv[argc - 1]))
      schedules.erase(dropout);

   cout << "all students, dropouts removed and sorted\n"
        << "first name last name: courses enrolled\n";
   printSchedules(schedules);

   // The set supplies the second required view and guarantees that each
   // enrolled student is printed exactly once.
   set<Student> enrolledStudents;
   for (const auto& entry : schedules)
      enrolledStudents.insert(entry.first);

   cout << "\nCurrently Enrolled Students\n";
   printStudents(enrolledStudents);
}

list<Student> readRoster(const string& fileName) {
   ifstream rosterFile(fileName);
   if (!rosterFile) {
      cerr << "Unable to open " << fileName << endl;
      std::exit(EXIT_FAILURE);
   }

   list<Student> roster;
   string firstName;
   string lastName;
   while (rosterFile >> firstName >> lastName)
      roster.emplace_back(firstName, lastName);

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

void printSchedules(const map<Student, list<string>>& schedules) {
   for (const auto& entry : schedules) {
      cout << entry.first.print() << ':';
      bool firstCourse = true;
      for (const string& course : entry.second) {
         if (!firstCourse)
            cout << ' ';
         cout << course;
         firstCourse = false;
      }
      cout << '\n';
   }
}

void printStudents(const set<Student>& students) {
   for (const Student& student : students)
      cout << student.print() << '\n';
}
