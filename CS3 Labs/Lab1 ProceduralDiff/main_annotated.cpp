#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int firstDifference(const string& line1, const string& line2) { 
    int shorterLength = static_cast<int>(line1.length());

    if (line2.length() < line1.length()) {
        shorterLength = static_cast<int>(line2.length());
    }

    for (int i = 0; i < shorterLength; i++) {
        if (line1[i] != line2[i]) {
            return i;
        }
    }

    if (line1.length() != line2.length()) {
        return shorterLength;
    }

    return -1;
}

void printDifference(const string& filename1, const string& filename2,
                     int lineNumber, const string& line1,
                     const string& line2) {
    int difference = firstDifference(line1, line2);
    if (difference == -1) {
        difference = 0;
    }

    string secondPrefix = filename2 + ": " + to_string(lineNumber) + ": ";
    string spaces(secondPrefix.length() + difference, ' ');

    cout << filename1 << ": " << lineNumber << ": " << line1 << endl;
    cout << filename2 << ": " << lineNumber << ": " << line2 << endl;
    cout << spaces << "^" << endl;
}

int main(int argc, char** argv) {
    // argc is the # of command line arguments, argv is the arguments themselves
    // if the program is ran like: .\main.cpp file1.txt file2.txt, then argv[1] is file1.txt and argv[2] is file2.txt
    // argc would be 3, because of the 3 arguments (file name, 1st argunement [file1], 2nd argument [file2])
    if (argc != 3) {
        cerr << "Error: enter exactly two file names." << endl;
        return 1;
    }

    //i fstream is "input file stream", used to read from files
    ifstream infile1(argv[1]); //opens filename stored in argv[1] (file1.txt) and connects it with infile1
    ifstream infile2(argv[2]); 

    if (!infile1 || !infile2) { //ifstream is an object in fstream, and has functionality to be treated for comparison
        cerr << "Error: could not open one or both files." << endl;
        return 1;
    }

    string line1;
    string line2;
    int lineNumber = 1;
    bool hasLine1 = static_cast<bool>(getline(infile1, line1)); //stores whether or not it could read the line (if there is a line to read) 
                                                                //as true or false. must use static cast because getline isnt a boolean function itself
    bool hasLine2 = static_cast<bool>(getline(infile2, line2));

    while (hasLine1 || hasLine2) { //while either file has a line left to read
        if (!hasLine1) {
            line1 = ""; //needs to print blank line 
        }
        if (!hasLine2) {
            line2 = "";
        }

        if (hasLine1 != hasLine2 || line1 != line2) {
            printDifference(argv[1], argv[2], lineNumber, line1, line2);
        }

        lineNumber++;
        hasLine1 = static_cast<bool>(getline(infile1, line1));
        hasLine2 = static_cast<bool>(getline(infile2, line2));
    }

    return 0;
}
