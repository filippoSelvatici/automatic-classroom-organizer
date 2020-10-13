#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "student.h"

class result {
   private:
    double friends_accuracy, first_preference_accuracy, second_preference_accuracy;
    int mark_max_delta;
    std::vector<int> ids_unsatisfied, color, females_each_class, repeating_each_class;

    bool is_satisfied(int id);

   public:
    result();
    result(std::vector<int>& _color);
    void update_color(std::vector<int>& _color);
    void calculate_parameters(std::vector<student>& students_data,
                              std::vector<std::vector<int> >& graph,
                              std::map<std::string, int>& surname_to_id, std::string& sections,
                              int N_CLASSES);
    void print_parameters(std::vector<student>& students_data, std::string& sections, std::ofstream& fout);
    friend bool operator>(const result& lhs, const result& rhs);
    friend bool operator>=(const result& lhs, const result& rhs);
};

double evaluate_friends(std::vector<std::vector<int> >& graph, std::vector<int>& color);
double evaluate_first_preference(std::vector<student>& students_data,
                                 std::map<std::string, int>& surname_to_id, std::vector<int>& color,
                                 std::string& sections);
double evaluate_second_preference(std::vector<student>& students_data,
                                  std::map<std::string, int>& surname_to_id,
                                  std::vector<int>& color, std::string& sections);
void evaluate_unsatisfied(std::vector<student>& students_data,
                          std::map<std::string, int>& surname_to_id, std::vector<int>& color,
                          std::string& sections);
int evaluate_mark_max_delta(std::vector<student>& students_data, std::vector<int>& color,
                            int N_CLASSES);
void evaluate_females(std::vector<student>& students_data, std::vector<int>& color, int N_CLASSES, std::vector<int>& females_each_class);
void evaluate_repeating(std::vector<student>& students_data, std::vector<int>& color, int N_CLASSES, std::vector<int>& repeating_each_class);
