#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "student.h"

int convert_mark(std::string& s);
void get_list_from(std::string& s, std::vector<std::string>& v);
void get_students_data_from_csv(std::vector<student>& students_data,
                                std::map<std::string, int>& surname_to_id);
void erase_friendship(std::vector<student>& students_data, int from, int to);
void create_graph(std::vector<student>& students_data, std::vector<std::vector<int> >& graph,
                  std::map<std::string, int>& surname_to_id);