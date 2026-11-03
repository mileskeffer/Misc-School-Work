// Roster with class schedule -- object-oriented version
// Based on rosterObject.cpp and the move constructor in moveIntArray.cpp.

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::list;
using std::string;

class Student {
public:
   Student(string firstName, string lastName)
      : firstName_(std::move(firstName)), lastName_(std::move(lastName)) {}

   // Retain ordinary copy behavior from rosterObject.cpp.
   Student(const Student& other) = default;

   // Required move constructor. The strings and course list transfer their
   // contents to this Student instead of copying every stored value.
   Student(Student&& other) noexcept
      : firstName_(std::move(other.firstName_)),
        lastName_(std::move(other.lastName_)),
        courses_(std::move(other.courses_)) {}

   void addCourse(const string& course) {
      // Avoid duplicate course labels if an input roster repeats a student.
      if (std::find(courses_.begin(), courses_.end(), course) == courses_.end())
         courses_.push_back(course);
   }

   void print() const {
      cout << firstName_ << ' ' << lastName_ << ':';
      for (const string& course : courses_)
         cout << course << ' ';
      cout << '\n';
   }

   // Student identity is based on the name, not on enrolled courses. This is
   // used both to merge course rosters and to remove dropouts.
   friend bool operator==(const Student& left, const Student& right) {
      return left.firstName_ == right.firstName_ &&
             left.lastName_ == right.lastName_;
   }

   // Sort by first name and then last name to match the supplied test output.
   friend bool operator<(const Student& left, const Student& right) {
      return left.firstName_ < right.firstName_ ||
             (left.firstName_ == right.firstName_ &&
              left.lastName_ < right.lastName_);
   }

private:
   string firstName_;
   string lastName_;
   list<string> courses_;
};

void readRoster(list<Student>& roster, const string& fileName);
void printRoster(const list<Student>& roster);
string courseName(const string& fileName);

int main(int argc, char* argv[]) {
   if (argc < 3) {
      cout << "usage: " << argv[0]
           << " list of courses, dropouts last" << endl;
      return EXIT_FAILURE;
   }

   list<Student> allStudents;

   for (int i = 1; i < argc - 1; ++i) {
      list<Student> courseRoster;
      readRoster(courseRoster, argv[i]);
      const string course = courseName(argv[i]);

      for (Student& student : courseRoster) {
         auto existing =
            std::find(allStudents.begin(), allStudents.end(), student);

         if (existing == allStudents.end()) {
            student.addCourse(course);

            // This explicitly demonstrates the move constructor: ownership of
            // the Student's strings and course list moves into allStudents.
            allStudents.push_back(std::move(student));
         } else {
            existing->addCourse(course);
         }
      }
   }

   cout << "All students\n"
        << "first name last name: courses enrolled\n";
   printRoster(allStudents);
   cout << "\n\n";

   list<Student> dropouts;
   readRoster(dropouts, argv[argc - 1]);
   for (const Student& dropout : dropouts)
      allStudents.remove(dropout);

   allStudents.sort();

   cout << "All students, dropouts removed and sorted\n"
        << "first name last name: courses enrolled\n";
   printRoster(allStudents);
}

void readRoster(list<Student>& roster, const string& fileName) {
   ifstream rosterFile(fileName);
   if (!rosterFile) {
      cerr << "Unable to open " << fileName << endl;
      std::exit(EXIT_FAILURE);
   }

   string firstName;
   string lastName;
   while (rosterFile >> firstName >> lastName)
      roster.emplace_back(firstName, lastName);
}

void printRoster(const list<Student>& roster) {
   for (const Student& student : roster)
      student.print();
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
