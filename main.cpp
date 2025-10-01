#include <iostream>
#include <string>
#include <iomanip>
#include <array>
#include <cstdint>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>

using namespace std;
string hashas(const std::string &input);
int collisions(int length);
string generateString(int length);


int main() {

    srand(time(0));

    cout<<"(1) Input string"<<endl;
    cout<<"(2) Input from a file"<<endl;
    cout<<"(3) Check for collisions"<<endl;

    int choice;
    cin >> choice;
    if(choice < 1 || choice > 3) {
        cout << "Invalid choice. Try again" << endl;
        cin>> choice;
    }

    switch (choice) {
        case 1: {
            string input;
            cout << "Enter string to hash: ";
            cin >> input;
            string hashed = hashas(input);
            cout << "Hashed string: " << hashed << endl;
            break;
        }
        case 2: {

            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            ifstream file(filename);
            if (!file) {
                cerr << "Error opening file." << endl;
                return 1;
            }
            ofstream fr("hashed_output.txt");
            if (!fr) {
                cerr << "Error creating output file." << endl;
                return 1;
            }
            int x;
            cout << "Enter number of lines to read: ";
            cin >> x;
             auto start = chrono::high_resolution_clock::now();
            string line;
            for (int i=0; i<x; i++) {
                getline(file, line);
                string hashed = hashas(line);
                fr<<i<< " | Hashed: " << hashed << endl;
            }
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = end - start;
            cout << "Time taken to hash " << x << " lines: " << elapsed.count() << " seconds" << endl;
            

            break;
        }
        case 3: {
            int length;
            cout << "Enter length of strings to compare: ";
            cin >> length;
            int collisionCount = collisions(length);
            cout << "Number of collisions: " << collisionCount << endl;
            break;
        }
        default:
            cout << "Invalid choice." << endl;
            break;
    }
    return 0;
}

string hashas(const std::string &input) {
    std::array<uint64_t, 4> state = {
        0xabcdef1234567890u, 0xfedcba9876543210u,
        0xa12b34c56d78e90fu, 0xa1b2c3d4e5f60708u
    };
    // sumaiso stringo charus i visus 4 states.
    for (size_t i = 0; i < input.size(); i++) {
        uint8_t ascii = static_cast<uint8_t>(input[i]);
        state[i % 4] ^= (static_cast<uint64_t>(ascii) * 0x0101010101010101u);  
        state[(i + 1) % 4] += (static_cast<uint64_t>(ascii) << (i % 56));
    }
    // kiekvienas state paveikia kitus
    for (int i = 0; i < 4; i++) {
        state[i] ^= state[(i + 1) % 4];
        state[i] *= 3;
    }
    // dar daugiau sumaisymo tarp states
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
int collisions(int length)
{   
    int count = 0;
    for(int i=0;i<1000000;i++)
    {
        string h1 = hashas(generateString(length));
        string h2 = hashas(generateString(length));
        if(h1 == h2)count++;

    }
    return count;
}

string generateString(int length) {

    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string result = "";
    
    for (int i = 0; i < length; i++) {
        int randomIndex = rand() % chars.length();  
        result += chars[randomIndex];
    }
    
    return result;
}