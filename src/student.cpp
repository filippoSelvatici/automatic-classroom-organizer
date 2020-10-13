#include "student.h"
#include <string>
#include <vector>

std::string student::data_to_string() {
    std::string s;
    char dsa = DSA + '0';
    char rep = repeating + '0';
    s = surname + " " + name + " " + std::to_string(id) + " " + std::to_string(mark) +
        " DSA: " + dsa + " repeating: " + rep + "\nfriends: ";
    for (auto x : friends) s += x + " ";
    s += "\nenemies: ";
    for (auto x : enemies) s += x + " ";
    s += "\nclasses: ";
    for (auto x : classes) s += x + " ";
    s += "\n";
    s += "\npreferences: ";
    for (auto x : preferences) s += x + " ";
    s += "\n";
    return s;
}