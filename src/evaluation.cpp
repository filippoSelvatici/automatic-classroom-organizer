#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <ctime>

#include "evaluation.h"
#include "student.h"

double evaluate_friends(std::vector<std::vector<int> >& graph, std::vector<int>& color) {
    double sum = 0, total = 0;
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            if (color[i] == color[graph[i][j]]) {
                sum += std::max(0.4, 1 - 0.2 * j);
            }
            total += std::max(0.4, 1 - 0.2 * j);
        }
    }
    return sum / total;
}

double evaluate_first_preference(std::vector<student>& students_data,
                                 std::map<std::string, int>& surname_to_id, std::vector<int>& color,
                                 std::string& sections) {
    double total = 0, sum = 0;
    for (auto x : students_data) {
        if (x.preferences.empty()) continue;
        total++;
        if (x.preferences[0] == "CLASSE") {
            for (int i = 0; i < x.classes.size(); i++) {
                if (color[x.id] == (x.classes[i][0] - sections[0] + 1)) {
                    sum++;
                    break;
                }
            }
        } else if (x.preferences[0] == "AMICI") {
            for (int i = 0; i < x.friends.size(); i++) {
                if (color[x.id] == color[surname_to_id[x.friends[i]]]) {
                    // sum+= 1/x.friends.size();
                    sum += (1 << (x.friends.size() - i - 1)) / ((1 << x.friends.size()) - 1);
                }
            }
        } else if (x.preferences[0] == "NEMICI") {
            for (int i = 0; i < x.enemies.size(); i++) {
                if (color[x.id] == color[surname_to_id[x.enemies[i]]]) {
                    // sum-= 1/x.enemies.size(); // maybe this should have more valor
                    sum -= (1 << (x.enemies.size() - i - 1)) / ((1 << x.enemies.size()) - 1);
                    // sum--;
                }
            }
        }
    }
    return sum / total;
}

double evaluate_second_preference(std::vector<student>& students_data,
                                  std::map<std::string, int>& surname_to_id,
                                  std::vector<int>& color, std::string& sections) {
    double total = 0, sum = 0;
    for (auto x : students_data) {
        if (x.preferences.size() <= 1) continue;
        total++;
        if (x.preferences[1] == "CLASSE") {
            for (int i = 0; i < x.classes.size(); i++) {
                if (color[x.id] == (x.classes[i][0] - sections[0] + 1)) {
                    sum++;
                    break;
                }
            }
        } else if (x.preferences[1] == "AMICI") {
            for (int i = 0; i < x.friends.size(); i++) {
                if (color[x.id] == color[surname_to_id[x.friends[i]]]) {
                    // sum+= 1/x.friends.size();
                    sum += (1 << (x.friends.size() - i - 1)) / ((1 << x.friends.size()) - 1);
                }
            }
        } else if (x.preferences[1] == "NEMICI") {
            for (int i = 0; i < x.enemies.size(); i++) {
                if (color[x.id] == color[surname_to_id[x.enemies[i]]]) {
                    // sum-= 1/x.enemies.size(); // maybe this should have more valor
                    sum -= (1 << (x.enemies.size() - i - 1)) / ((1 << x.enemies.size()) - 1);
                    // sum--;
                }
            }
        }
    }
    return sum / total;
}

void evaluate_unsatisfied(std::vector<student>& students_data,
                          std::map<std::string, int>& surname_to_id, std::vector<int>& color,
                          std::string& sections, std::vector<int>& ids_unsatisfied) {
    for (int i = 0; i < students_data.size(); i++) {
        if (students_data[i].preferences.empty()) continue;
        bool satisfied = false;
        if (!students_data[i].friends.empty()) {
            for (auto x : students_data[i].friends) {
                if (color[i] == color[surname_to_id[x]]) {
                    satisfied = true;
                }
            }
        }
        if (!students_data[i].classes.empty()) {
            for (auto x : students_data[i].classes) {
                if (color[i] == x[0] - sections[0] + 1) {
                    satisfied = true;
                }
            }
        }
        if (!students_data[i].enemies.empty()) {
            for (auto x : students_data[i].enemies) {
                if (color[i] != color[surname_to_id[x]]) {
                    satisfied = true;
                }
            }
        }
        if (!satisfied) {
            ids_unsatisfied.push_back(i);
        }
    }
    return;
}

int evaluate_mark_max_delta(std::vector<student>& students_data, std::vector<int>& color,
                            int N_CLASSES) {
    int cont[N_CLASSES][11] = {0}, max_delta = 0;
    for (int i = 0; i < color.size(); i++) {
        cont[color[i] - 1][students_data[i].mark]++;
    }
    for (int i = 0; i < N_CLASSES; i++) {
        for (int j = i + 1; j < N_CLASSES; j++) {
            for (int mark = 6; mark <= 10; mark++) {
                max_delta = std::max(abs(cont[i][mark] - cont[j][mark]), max_delta);
            }
        }
    }
    return max_delta;
}

result::result() {
    friends_accuracy = 0;
    first_preference_accuracy = 0;
    second_preference_accuracy = 0;
    mark_max_delta = 2;
    ids_unsatisfied.resize(1000);
}

result::result(std::vector<int>& _color) {
    friends_accuracy = 0;
    first_preference_accuracy = 0;
    second_preference_accuracy = 0;
    mark_max_delta = 100;
    color = _color;
}

void result::update_color(std::vector<int>& _color) { color = _color; }

void result::calculate_parameters(std::vector<student>& students_data,
                                  std::vector<std::vector<int> >& graph,
                                  std::map<std::string, int>& surname_to_id, std::string& sections,
                                  int N_CLASSES) {
    friends_accuracy = evaluate_friends(graph, this->color) * 100;
    first_preference_accuracy =
        evaluate_first_preference(students_data, surname_to_id, this->color, sections) * 100;
    second_preference_accuracy =
        evaluate_second_preference(students_data, surname_to_id, this->color, sections) * 100;
    mark_max_delta = evaluate_mark_max_delta(students_data, this->color, N_CLASSES);
    evaluate_unsatisfied(students_data, surname_to_id, this->color, sections,
                         this->ids_unsatisfied);
    evaluate_females(students_data, this->color, N_CLASSES, this->females_each_class);
    evaluate_repeating(students_data, this->color, N_CLASSES, this->repeating_each_class);
}

bool result::is_satisfied(int id) {
    for(auto x: ids_unsatisfied) {
        if(x == id) {
            return false;
        }
    }
    return true;
}

void result::print_parameters(std::vector<student>& students_data, std::string& sections, std::ofstream& fout) {
    time_t now = time(0);
    char* date_time = ctime(&now);
    fout << "Data locale e orario locale: " << date_time << "\n\n";
    fout << "Accuratezza amicizie: " << friends_accuracy << "\n";
    fout << "Accuratezza prima preferenza: " << first_preference_accuracy << "\n";
    fout << "Accuratezza seconda preferenza: " << second_preference_accuracy << "\n";
    fout << "Massima differenza di voti: " << mark_max_delta << "\n";
    fout << "Numero di insoddisfatti: " << ids_unsatisfied.size() << "\n\n";
    for(int i=0; i<sections.size(); i++) {
        fout << "Sezione " << sections[i] << "\tNumero ripetenti: " << repeating_each_class[i] << "\tNumero femmine: " << females_each_class[i] << "\n";
        for(int j=0; j<color.size(); j++) {
            if(i == color[j]-1) {
                fout << students_data[j].surname + " " + students_data[j].name;
                if(students_data[j].repeating)
                    fout << " ripetente";
                if(students_data[j].gender == 'F')
                    fout << " femmina";
                if(!is_satisfied(j))
                    fout << " insoddisfatto";
                fout << "\n";
            }
        }
        fout << "\n";
    }
    fout << "\n\n\n";
}

void evaluate_females(std::vector<student>& students_data, std::vector<int>& color, int N_CLASSES, std::vector<int>& females_each_class) {
    females_each_class.resize(N_CLASSES);
    for(int i=0; i<color.size(); i++) {
        if(students_data[i].gender == 'F') {
            females_each_class[color[i]-1]++;
        }
    }
}

void evaluate_repeating(std::vector<student>& students_data, std::vector<int>& color, int N_CLASSES, std::vector<int>& repeating_each_class) {
    repeating_each_class.resize(N_CLASSES);
    for(int i=0; i<color.size(); i++) {
        if(students_data[i].repeating) {
            repeating_each_class[color[i]-1]++;
        }
    }
}

bool operator>(const result& lhs, const result& rhs) {
    if (lhs.friends_accuracy > rhs.friends_accuracy &&
        lhs.first_preference_accuracy > rhs.first_preference_accuracy &&
        lhs.second_preference_accuracy > rhs.second_preference_accuracy &&
        lhs.mark_max_delta < rhs.mark_max_delta &&
        lhs.ids_unsatisfied.size() < rhs.ids_unsatisfied.size()) {
        return true;
    }
    return false;
}

bool operator>=(const result& lhs, const result& rhs) {
    if (lhs.friends_accuracy >= rhs.friends_accuracy &&
        lhs.first_preference_accuracy >= rhs.first_preference_accuracy &&
        lhs.second_preference_accuracy >= rhs.second_preference_accuracy &&
        lhs.mark_max_delta <= rhs.mark_max_delta &&
        lhs.ids_unsatisfied.size() <= rhs.ids_unsatisfied.size()) {
        return true;
    }
    return false;
}