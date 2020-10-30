#pragma once
#include <string>
#include <utility>
#include <vector>

#include "student.h"

void assign_color(std::vector<student>& students_data, std::vector<std::vector<int> >& graph,
                  std::vector<int>& color, std::vector<int>& class_capacity_left,
                  std::string& sections, int N_CLASSES);
int get_most_capacios_class(std::vector<int>& class_capacity_left, int N_CLASSES);
std::pair<int, int> get_node(std::vector<int>& color, std::vector<std::queue<int> >& q,
                             std::vector<int>& class_capacity_left, int N_CLASSES,
                             int& not_colored_nodes);
void assign_color_DSA(std::vector<student>& students_data, std::vector<int>& color,
                      std::vector<std::queue<int> >& q, std::vector<int>& class_capacity_left,
                      std::string& sections);
void assign_color_class_preference(std::vector<student>& students_data, std::vector<int>& color,
                                   std::vector<std::queue<int> >& q,
                                   std::vector<int>& class_capacity_left, std::string& sections);
void assign_color_repeating(std::vector<student>& students_data, std::vector<int>& color,
                            std::vector<std::queue<int> >& q, std::vector<int>& class_capacity_left,
                            std::string& sections, int N_CLASSES);
