#include <iostream>

using namespace std;

uint64_t factorial(uint16_t x) {
    if (x < 2) return 1;
    else {
        return (x * factorial(x-1));
    }
}

int main() {
    cout << "Hello factorial!" << endl;
    cout << factorial(60);
    return 0;
}