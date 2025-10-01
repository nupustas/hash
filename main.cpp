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
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;
string hashas(const std::string &input);
int collisions(int length);
string generateString(int length);
void avalancheEffect();


int main() {

    srand(time(0));

    cout<<"(1) Input string"<<endl;
    cout<<"(2) Input from a file"<<endl;
    cout<<"(3) Check for collisions"<<endl;
    cout<<"(4) Avalanche effect"<<endl;

    int choice;
    cin >> choice;

    if(choice < 1 || choice > 4) {
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
            ofstream fs("a1111.txt");
            /*for(int x=0;x<1111;x++)
            {
                fs<<generateString(1);
            }*/
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

            int x;
            cout << "Enter number of lines to read: ";
            cin >> x;
            //ofstream fr("hash_test.txt");
            auto start = chrono::high_resolution_clock::now();

            string line, combined;
            for (int i = 0; i < x; i++) {
                getline(file, line);
                //fr << line << endl;
                combined += line + "\n";  // preserve line breaks
            }

            string hashed = hashas(combined); // hash the entire combined string
            cout << "Hashed block: " << hashed << endl;

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = end - start;
            cout << "Time taken to hash " << x << " lines: " 
                << elapsed.count() << " seconds" << endl;

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
        case 4: {
            avalancheEffect();
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

void avalancheEffect() {
    const int PAIRS = 100000;
    vector<double> bitDiffs, hexDiffs;
    
    for (int i = 0; i < PAIRS; i++) {
        string s1 = generateString(15), s2 = s1;
        int pos = rand() % s2.length();
        char originalChar = s2[pos];
        char newChar;
        do {
            newChar = 'a' + rand() % 26;
        } while (newChar == originalChar); 
        
        s2[pos] = newChar;
        
        string h1 = hashas(s1);
        string h2 = hashas(s2);

        int bitDiff = 0, hexDiff = 0;
        for (int j = 0; j < 64; j++) {
            if (h1[j] != h2[j]) hexDiff++;
            int v1 = h1[j] > '9' ? h1[j] - 'a' + 10 : h1[j] - '0';
            int v2 = h2[j] > '9' ? h2[j] - 'a' + 10 : h2[j] - '0';
            bitDiff += __builtin_popcount(v1 ^ v2);//suskaiciuoja kiek bitu skiriasi
        }
        
        bitDiffs.push_back(bitDiff * 100.0 / 256);
        hexDiffs.push_back(hexDiff * 100.0 / 64);
    }
    
    auto [minBit, maxBit] = minmax_element(bitDiffs.begin(), bitDiffs.end());
    auto [minHex, maxHex] = minmax_element(hexDiffs.begin(), hexDiffs.end());
    
    double avgBit = accumulate(bitDiffs.begin(), bitDiffs.end(), 0.0) / PAIRS;
    double avgHex = accumulate(hexDiffs.begin(), hexDiffs.end(), 0.0) / PAIRS;

    cout << "min bits diff: " << *minBit << "% - max bits diff: " << *maxBit << "% (avg bits diff: " << avgBit << "%)\n";
    cout << "min hex diff: " << *minHex << "% - max hex diff: " << *maxHex << "% (avg hex diff: " << avgHex << "%)\n";
}