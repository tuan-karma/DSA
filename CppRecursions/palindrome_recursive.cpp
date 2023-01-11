#include <iostream>

using namespace std;

int last(int x) {
    return x % 10;
}

int remain(int x) {
    return x / 10;
}

int reverse(int x, int acc = 0) {
    if (x == 0) return acc; // base case

    acc = (acc * 10) + last(x);
    return reverse(remain(x), acc);
}

bool isPalindrome(int x) {
    return (x == reverse(x));
}


int main() {

    cout << "Is 6 a palindrome? " << isPalindrome(6) << endl;
    cout << "Is 262 a palindrome? " << isPalindrome(262) << endl;
    cout << "Is 22 a palindrome? " << isPalindrome(22) << endl;
    cout << "Is 0 a palindrome? " << isPalindrome(0) << endl;
    cout << "Is 123 a palindrome? " << isPalindrome(123) << endl;
    cout << "Is 1234321 a palindrome? " << isPalindrome(1234321) << endl;
    cout << "Is 123321 a palindrome? " << isPalindrome(123321) << endl;
    cout << "Is 66996699 a palindrome? " << isPalindrome(66996699) << endl;
    cout << "Is 0066996600 a palindrome? " << isPalindrome(66996600) << endl;

    return 0;
}