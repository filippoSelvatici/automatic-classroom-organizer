#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "input.h"
#include "rapidcsv.h"
#include "student.h"

int convert_mark(std::string& s) {
    if (s == "SEI" || s == "6")
        return 6;
    else if (s == "SETTE" || s == "7")
        return 7;
    else if (s == "OTTO" || s == "8")
        return 8;
    else if (s == "NOVE" || s == "9")
        return 9;
    else if (s == "DIECI" || s == "10" || s == "10 e lode")
        return 10;
    else
        return 6;
}

void to_upper(std::string& s) {
    // creates a copy of the string passed as parameter, convert
    // it to uppercase and return it
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return;
}

void get_list_from(std::string& s, std::vector<std::string>& v) {
    std::stringstream sstream(s);
    std::string segment;
    while (getline(sstream, segment, ';')) {
        to_upper(segment);
        v.push_back(segment);
    }
}

void get_students_data_from_csv(std::vector<student>& students_data,
                                std::map<std::string, int>& surname_to_id) {
    rapidcsv::Document doc("data/csv_with_data.csv");
    std::vector<std::string> surnames = doc.GetColumn<std::string>("Cognome");
    std::vector<std::string> names = doc.GetColumn<std::string>("Nome");
    std::vector<std::string> marks = doc.GetColumn<std::string>("Voto");
    std::vector<std::string> DSA = doc.GetColumn<std::string>("dsa o H");
    std::vector<char> genders = doc.GetColumn<char>("Sesso");
    std::vector<std::string> friends = doc.GetColumn<std::string>("Amici");
    std::vector<std::string> classes = doc.GetColumn<std::string>("Classe");
    std::vector<std::string> enemies = doc.GetColumn<std::string>("Nemici");
    std::vector<std::string> priority = doc.GetColumn<std::string>("Preferenza");
    std::vector<std::string> repeating = doc.GetColumn<std::string>("Ripetente");

    for (int i = 0; i < surnames.size(); i++) {
        student temp;
        to_upper(surnames[i]);
        to_upper(names[i]);
        to_upper(marks[i]);
        to_upper(DSA[i]);
        to_upper(repeating[i]);
        temp.surname = surnames[i];
        temp.name = names[i];
        temp.mark = convert_mark(marks[i]);
        temp.gender = std::toupper(genders[i]);
        temp.DSA = (DSA[i] == "DSA");
        temp.repeating = (repeating[i] == "SI");
        if (temp.repeating) temp.mark = 6;
        get_list_from(friends[i], temp.friends);
        get_list_from(classes[i], temp.classes);
        get_list_from(enemies[i], temp.enemies);
        get_list_from(priority[i], temp.preferences);
        students_data.push_back(temp);
    }
    std::sort(students_data.begin(), students_data.end(), 
    	[] (const student& lhs, const student& rhs) {
    		if(lhs.surname == rhs.surname)
    			return lhs.name < rhs.name;
    		return lhs.surname < rhs.surname;
    });

    for(int i=0; i<students_data.size(); i++) {
    	students_data[i].id = i;
        surname_to_id[students_data[i].surname] = i;
    }
}

void erase_friendship(std::vector<student>& students_data, int from, int to) {
    for (int i = 0; i < students_data[from].friends.size(); i++) {
        if (students_data[from].friends[i] == students_data[to].surname) {
            students_data[from].friends.erase(students_data[from].friends.begin() + i);
            return;
        }
    }
}

void create_graph(std::vector<student>& students_data, std::vector<std::vector<int> >& graph,
                  std::map<std::string, int>& surname_to_id) {
    graph.resize(students_data.size());
    std::vector<std::string> friends_curr, enemies_curr;

    for (int i = 0; i < students_data.size(); i++) {
        for (auto x : students_data[i].friends) {
            if (surname_to_id.find(x) != surname_to_id.end()) {
                friends_curr.push_back(x);
            }
        }
        for (auto x : students_data[i].enemies) {
            if (surname_to_id.find(x) != surname_to_id.end()) {
                enemies_curr.push_back(x);
            }
        }
        students_data[i].friends = friends_curr;
        students_data[i].enemies = enemies_curr;
        friends_curr.clear();
        enemies_curr.clear();
    }

    for (int i = 0; i < students_data.size(); i++) {
        for (auto x : students_data[i].enemies) {
            erase_friendship(students_data, surname_to_id[x], i);
        }
    }

    for (int i = 0; i < graph.size(); i++) {
        for (auto x : students_data[i].friends) {
            graph[i].push_back(surname_to_id[x]);
        }
    }
    return;
}