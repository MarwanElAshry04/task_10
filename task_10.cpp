#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "des.h"

using namespace std;

extern "C" uint64_t des(uint64_t input, uint64_t key, char mode);

uint64_t des25(uint64_t block, uint64_t key) {
    for (int i = 0; i < 25; ++i)
        block = des(block, key, 'e');
    return block;
}

uint64_t password_to_key(const string& pwd) {
    uint64_t key = 0;
    for (int i = 0; i < 8 && i < (int)pwd.size(); ++i)
        key |= (uint64_t(uint8_t(pwd[i])) << ((7 - i) * 8));
    return key;
}

uint64_t salted_block(uint16_t salt) {
    return ((uint64_t)salt << 48);
}

string to_hex(uint64_t val) {
    ostringstream oss;
    oss << hex << setw(16) << setfill('0') << val;
    return oss.str();
}

uint64_t from_hex(const string& hex) {
    uint64_t val;
    istringstream iss(hex);
    iss >> std::hex >> val;
    return val;
}

int main() {
    srand((unsigned)time(nullptr));

    vector<string> passwords = {
        "coffee123", "sunshine!", "myDogLuna", "booklover",
        "passw0rd", "ilikecats", "summer2023", "guitarhero",
        "vanilla99", "spaghetti"
    };

    cout << "Encrypted passwords:\n";
    for (const auto& pwd : passwords) {
        uint16_t salt = rand() & 0xFFFF;
        uint64_t block = salted_block(salt);
        uint64_t key = password_to_key(pwd);
        uint64_t encrypted = des25(block, key);
        cout << hex << setw(4) << setfill('0') << salt
            << ":" << to_hex(encrypted) << "\n";
    }

    string hash;
    cout << "\nCheck a password against a stored hash.\n";
    cout << "Paste salt and cipher in this format (salt:cipher): ";
    getline(cin, hash);

    auto colon = hash.find(':');
    if (colon != string::npos) {
        uint16_t salt = stoi(hash.substr(0, colon), nullptr, 16);
        uint64_t stored = from_hex(hash.substr(colon + 1));

        string attempt;
        cout << "Enter the password to test: ";
        getline(cin, attempt);

        uint64_t key = password_to_key(attempt);
        uint64_t encrypted = des25(salted_block(salt), key);

        if (encrypted == stored)
            cout << "\n Password is correct!\n";
        else
            cout << "\n Password does not match.\n";
    }

    return 0;
}
