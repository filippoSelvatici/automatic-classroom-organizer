#pragma once
#include <string>
#include <vector>

class student {
   public:
    bool DSA, repeating;
    char gender;
    int id, mark;
    std::string name, surname;
    std::vector<std::string> enemies, friends, classes, preferences;
    // where do he live?
    std::string data_to_string();
};