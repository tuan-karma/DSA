#include <iostream>

using namespace std;

uint64_t factorial(uint16_t x) {
    uint64_t result = 1;
    while (x > 1) {
        result = result * x;
        x = x - 1;
    }
    return result;
}

int main() {
    cout << factorial(100);
    return 0;
}