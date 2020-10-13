// ALGORITMO GREEDY
#include <assert.h>
#include <time.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "automatic_classroom_creator.h"
#include "evaluation.h"
#include "input.h"
#include "rapidcsv.h"
#include "student.h"

void fill_color_with_done_classes(std::vector<int> &color) {
    int color2[] = {5, 1, 2, 2, 1, 3, 3, 5, 2, 1, 5, 1, 4, 5, 2, 3, 3, 1, 2, 4, 1, 5, 1,
                    2, 3, 5, 5, 4, 2, 5, 5, 4, 2, 1, 1, 4, 3, 5, 2, 1, 1, 3, 4, 3, 4, 2,
                    1, 4, 2, 4, 5, 4, 5, 3, 2, 2, 2, 4, 4, 1, 2, 4, 1, 4, 5, 4, 5, 3, 3,
                    2, 1, 4, 4, 3, 1, 2, 5, 1, 5, 1, 2, 3, 3, 2, 3, 4, 1, 5, 2, 5, 4, 4,
                    1, 1, 5, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5};
    for (int i = 0; i < color.size(); i++) {
        color[i] = color2[i];
    }
    return;
}

void print_classes(std::vector<student> &students_data, std::string &sections) {
    std::ofstream los("list_of_students.txt");
    int c2[] = {3, 3, 2, 3, 2, 1, 2, 3, 1, 1, 2, 1, 1, 4, 4, 3, 4, 1, 1, 4, 2, 1, 4, 4,
                3, 4, 2, 1, 3, 3, 1, 1, 3, 2, 4, 4, 4, 2, 4, 1, 3, 1, 4, 3, 1, 1, 2, 4,
                2, 4, 3, 4, 1, 4, 4, 4, 2, 2, 3, 2, 3, 3, 3, 4, 3, 2, 4, 2, 2, 2, 2, 4,
                1, 1, 3, 1, 3, 4, 1, 4, 3, 2, 1, 2, 4, 2, 1, 3, 3, 2, 1, 3, 2};
    for (int i = 0; i < sections.size(); i++) {
        los << "Sezione " << sections[i] << ":\n";
        for (int j = 0; j < students_data.size(); j++) {
            if (c2[j] == i + 1) {
                los << students_data[j].surname << "\n";
            }
        }
        los << "\n";
    }
}

int main() {
    std::string sections;
    int N_CLASSES;

    std::vector<student> students_data;
    std::map<std::string, int> surname_to_id;

    std::vector<std::vector<int>> graph;
    // graph of friendships. Some nodes may have no edges
    // The edge (node; v[node][0]) is more important than (node; v[node][1])

    std::vector<int> color;
    // node x belongs to the class 0 < color[x] <= N_CLASSES
    // std::vector color is used as visited flag

    // NB: color.size() == graph.size()

    std::vector<int> class_capacity_left;
    std::vector<int> class_capacity_left_tmp;
    // class_capacity_left[id]: id == 0 not defined, 1 <= id <= N_CLASSES defined

    std::srand(time(NULL));
    std::cout << "Quante classi vuoi per questo indirizzo? ";
    std::cin >> N_CLASSES;
    std::cout << "Quali sezioni vuoi per questo indirizzo? (indicare le lettere una di seguito "
                 "all'altra, ad es. ABC) ";
    std::getline(std::cin, sections);

    while (sections.find(' ') != std::string::npos) sections.erase(sections.find(' '), 1);
    std::cerr << "\nSections are: " + sections + "\n\n" << std::flush;

    class_capacity_left.resize(N_CLASSES + 1);
    class_capacity_left_tmp.resize(N_CLASSES + 1);
    for (int i = 1; i <= N_CLASSES; i++) {
        std::cout << "Quanti alunni nella sezione " << sections[i - 1] << "?\n";
        std::cin >> class_capacity_left_tmp[i];
    }

    get_students_data_from_csv(students_data, surname_to_id);
    std::cerr << "Students data saved successfully.\n";
    // std::cout << students_data[28].data_to_string() << "\n";
    // print_classes(students_data, sections);
    // return 0;
    // create graph of friendships
    create_graph(students_data, graph, surname_to_id);
    std::cerr << "Graph created successfully.\n";
    std::ofstream fout("data/out.txt", std::ofstream::app);
    bool found = false;
    result best;
	for (int times = 0; times < 10000; times++) {
	    for (int i = 0; i < class_capacity_left.size(); i++) {
	        class_capacity_left[i] = class_capacity_left_tmp[i];
	    }
	    assign_color(students_data, graph, color, class_capacity_left, sections, N_CLASSES);
	    result actual_result(color);
	    actual_result.calculate_parameters(students_data, graph, surname_to_id, sections,
	                                       N_CLASSES);
	    if (actual_result >= best) {
	    	found = true;
	        best = actual_result;
	    }
	}
	if(found) {
    		best.print_parameters(students_data, sections, fout);
	} else {
		std::cerr << "Found nothing!";
	}
    return 0;
}
