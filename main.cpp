
#include <iostream>
#include <string>
#include <iomanip>

using std::string;
using std::cout;
using std::cin;
using std::endl;

int hashas(const string& input);

int main() {
    string input;
    cout << "Enter password" << endl;
    cin >> input;

    int hashed = hashas(input);
    cout << "Hashed password: " << hashed << endl;
    return 0;
}

int hashas(const string& input) {
    int hash = 0;
    for (char c : input) {
        hash += static_cast<int>(c);
    }
    return hash;
}