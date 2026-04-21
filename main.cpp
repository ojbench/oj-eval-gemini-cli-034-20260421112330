#include <iostream>
#include <string>
#include "SkipList.hpp"

int main() {
    SkipList<int> sl;
    int n;
    if (!(std::cin >> n)) return 0;
    for (int i = 0; i < n; ++i) {
        int op, x;
        std::cin >> op >> x;
        if (op == 1) {
            sl.insert(x);
        } else if (op == 2) {
            if (sl.search(x)) {
                std::cout << "True" << std::endl;
            } else {
                std::cout << "False" << std::endl;
            }
        } else if (op == 3) {
            sl.deleteItem(x);
        }
    }
    return 0;
}
