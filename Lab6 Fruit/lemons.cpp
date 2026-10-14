// picking the best quality lemons
// converting vectors to priority queue
// Mikhail Nesterenko
// 2/17/2023


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <queue>

using namespace std;

struct Lemon {
    double quality;
    friend bool operator<(const Lemon& left, const Lemon& right) {
        return left.quality < right.quality;
    }
};

const int maxBoxSize = 30;
const int highestQuality = 10;

int main() {
    srand(time(nullptr));
    priority_queue<Lemon> box;
    const int boxSize = rand() % maxBoxSize + 1;

    // random quality assignment
    for (int i = 0; i < boxSize; ++i)
        box.push({static_cast<double>(rand()) / RAND_MAX * highestQuality});

    cout << "Here are the lemons (best first): ";

    while (!box.empty()) {
        cout << box.top().quality << ", ";
        box.pop();
    }
    cout << endl;
}

