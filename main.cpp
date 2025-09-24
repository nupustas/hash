
#include <iostream>
#include <string>
#include <iomanip>
#include <array>
#include <cstdint>
#include <sstream>

using std::string;
using std::cout;
using std::cin;
using std::endl;

string hashas(const std::string &input);

int main() {
    string input;
    cout << "Enter password" << endl;
    cin >> input;

    string hashed = hashas(input);
    cout << "Hashed password: " << hashed << endl;
    return 0;
}

string hashas(const std::string &input) {
    std::array<uint32_t, 8> state = {
        0x12345678u, 0x9abcdef0u, 0x0fedcba9u, 0x87654321u,
        0xf0e1d2c3u, 0x4b5a6978u, 0x11223344u, 0x55667788u
    };

    for (size_t i = 0; i < input.size(); i++) {
        uint8_t c = static_cast<uint8_t>(input[i]);
        state[i % 8] ^= (c * 0x01010101u);  
        state[(i + 1) % 8] += (c << (i % 24));
    }

    // konvertuoja 8x32bit i 256bit hash stringa
    std::ostringstream oss;
    for (uint32_t v : state) {
        oss << std::hex << std::setw(8) << std::setfill('0') << v;
    }
    return oss.str();
}