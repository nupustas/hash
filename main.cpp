
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
    std::array<uint64_t, 4> state = {
        0xabcdef1234567890u, 0xfedcba9876543210u,
        0xa12b34c56d78e90fu, 0xa1b2c3d4e5f60708u
    };
    // sumaiso stringo charus i visus 4 states.
    for (size_t i = 0; i < input.size(); i++) {
        uint8_t c = static_cast<uint8_t>(input[i]);
        state[i % 4] ^= (static_cast<uint64_t>(c) * 0x0101010101010101u);  
        state[(i + 1) % 4] += (static_cast<uint64_t>(c) << (i % 56));
    }
    // kiekvienas state paveikia kitus
    for (int i = 0; i < 4; i++) {
        state[i] ^= state[(i + 1) % 4];
        state[i] *= 3;
    }
    // dar daugiau sumaisymo
    for (int i = 0; i < 4; i++) {
        state[i] = state[i] + state[(i + 2) % 4];
        state[i] = state[i] ^ (state[i] >> 16);
    }

    // convert 4x64bit = 256bit i 64 simboliu hex stringa
    std::ostringstream of;
    for (uint64_t v : state) {
        of << std::hex << std::setw(16) << std::setfill('0') << v;
    }
    return of.str();
}