#include <iostream>
#include <list>
#include <iterator> // Required for std::advance, next, prev, and distance

int main() {
    // Create a list container (Bidirectional iterator, does not support random access like list.begin() + 3)
    std::list<int> numbers = {10, 20, 30, 40, 50, 60, 70};

    // Initialize an iterator to the starting point
    auto it = numbers.begin();
    std::cout << "1. Starting at begin(): " << *it << "\n\n";

    // -------------------------------------------------------------------------
    // std::advance(iterator, n)
    // Modifies the iterator in-place. Moves it forward or backward by n steps.
    // -------------------------------------------------------------------------
    std::advance(it, 3); // Moves 'it' 3 positions forward (pointing to 40)
    std::cout << "2. After std::advance(it, 3):\n";
    std::cout << "   Current element (modifies original): " << *it << "\n\n";

    // -------------------------------------------------------------------------
    // std::next(iterator, n)
    // Returns a copy of the iterator advanced by n steps. Does NOT modify original.
    // -------------------------------------------------------------------------
    auto next_it = std::next(it, 2); // Looks 2 positions ahead from 40 (points to 60)
    std::cout << "3. Using std::next(it, 2):\n";
    std::cout << "   Original iterator stays at: " << *it << "\n";
    std::cout << "   Returned forward iterator: " << *next_it << "\n\n";

    // -------------------------------------------------------------------------
    // std::prev(iterator, n)
    // Returns a copy of the iterator moved backward by n steps. Does NOT modify original.
    // -------------------------------------------------------------------------
    auto prev_it = std::prev(it, 1); // Looks 1 position behind from 40 (points to 30)
    std::cout << "4. Using std::prev(it, 1):\n";
    std::cout << "   Original iterator stays at: " << *it << "\n";
    std::cout << "   Returned backward iterator: " << *prev_it << "\n\n";

    // -------------------------------------------------------------------------
    // std::distance(first, last)
    // Calculates the number of elements between two iterators.
    // -------------------------------------------------------------------------
    auto start = numbers.begin();
    auto end = numbers.end();
    
    auto total_dist = std::distance(start, end);
    auto partial_dist = std::distance(start, it);

    std::cout << "5. Using std::distance:\n";
    std::cout << "   Total distance (size of container): " << total_dist << "\n";
    std::cout << "   Distance from begin() to current position ('it'): " << partial_dist << "\n";

    return 0;
}
