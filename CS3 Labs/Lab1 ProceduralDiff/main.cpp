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
    cout << spaces << "^" << endl << endl;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Error: enter exactly two file names." << endl;
        return 1;
    }

    ifstream infile1(argv[1]);
    ifstream infile2(argv[2]);

    if (!infile1 || !infile2) {
        cerr << "Error: could not open one or both files." << endl;
        return 1;
    }

    string line1;
    string line2;
    int lineNumber = 1;
    bool hasLine1 = static_cast<bool>(getline(infile1, line1));
    bool hasLine2 = static_cast<bool>(getline(infile2, line2));

    while (hasLine1 || hasLine2) {
        if (!hasLine1) {
            line1 = "";
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
