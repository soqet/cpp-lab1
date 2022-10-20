#include "linkedhs.hpp"

#include <iostream>

struct fHashInt {
    size_t operator()(const int &v) {
        return v;
    }
};

int main() {
    auto test = Linkedhs<int, fHashInt>(10);
    test.insert(10);
    test.insert(11);
    test.insert(100);
    auto test2 = new Linkedhs<int, fHashInt>(test);
    test2->insert(-2);
    test.remove(11);
    for (auto i = test.begin(); i != test.end(); i++) {
        std::cout << *i << std::endl;
    }
    std::cout << std::endl;
    for (auto i = test2->begin(); i != test2->end(); i++) {
        std::cout << *i << std::endl;
    }
    return 0;
}
