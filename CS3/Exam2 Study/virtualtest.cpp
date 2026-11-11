#include <iostream>
#include <functional>

using std::cout;
using std::endl;
using std::cin;
using std::plus;
using std::minus;
using std::multiplies;
using std::divides;

class Calculator {
public:
    virtual ~Calculator() = default;
    virtual double calculate(double a, double b) const = 0;
};

class Addition : public Calculator {
public:
    double calculate(double a, double b) const override {
        return plus<double>()(a, b);
    }
};

class Subtraction : public Calculator {
public:
    double calculate(double a, double b) const override {
        return minus<double>()(a, b);
    }
};

class Multiply : public Calculator {
public:
    double calculate(double a, double b) const override {
        return multiplies<double>()(a, b);
    }
};

class Divide : public Calculator {
public:
    double calculate(double a, double b) const override {
        return divides<double>()(a, b);
    }
};

int main() {

    int Quit = 0;
    while (Quit == 0) {

        cout << "select operation:[A]dd [S]ubtract [M]ultiply [D]ivide [Q]uit: ";
        char choice;
        cin >> choice;

        Calculator* calc = nullptr;

        switch (choice) {
            case 'A':
            case 'a':
                calc = new Addition();
                break;
            case 'S':
            case 's':
                calc = new Subtraction();
                break;
            case 'M':
            case 'm':
                calc = new Multiply();
                break;
            case 'D':
            case 'd':
                calc = new Divide();
                break;
            case 'Q':
            case 'q':
                cout << "Exiting the program." << endl;
                return 0;
            default:
                cout << "Invalid choice!" << endl;
                continue;
        }

        cout << "Enter two numbers: ";
        double a, b;
        cin >> a >> b;

        cout << "Result: " << calc->calculate(a, b) << endl;

        delete calc;
    }
}
