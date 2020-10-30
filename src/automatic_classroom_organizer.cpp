#include <assert.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "automatic_classroom_organizer.h"
#include "student.h"

void assign_color(std::vector<student>& students_data, std::vector<std::vector<int> >& graph,
                  std::vector<int>& color, std::vector<int>& class_capacity_left,
                  std::string& sections, int N_CLASSES) {
    std::vector<std::queue<int> > q;
    // queues of nodes

    // initialize color
    color.resize(students_data.size());
    std::fill(color.begin(), color.end(), 0);

    int max_out_degree_node = 0;
    for (int i = 0; i < graph.size(); i++) {
        max_out_degree_node = std::max(max_out_degree_node, (int)graph[i].size());
    }

    q.resize(max_out_degree_node);

    // assign DSA students
    assign_color_DSA(students_data, color, q, class_capacity_left, sections);
    // assign students with class priority
    assign_color_class_preference(students_data, color, q, class_capacity_left, sections);
    // assign repeating students
    assign_color_repeating(students_data, color, q, class_capacity_left, sections, N_CLASSES);

    int not_colored_nodes = 0;
    for (int i = 0; i < color.size(); i++) {
        not_colored_nodes += (color[i] == 0);
    }

    while (not_colored_nodes) {
        std::pair<int, int> actual =
            get_node(color, q, class_capacity_left, N_CLASSES, not_colored_nodes);
        int node = actual.first;
        int nth_choice = actual.second;
        if (node == -1) break;
        if (class_capacity_left[color[node]] == 0) continue;
        for (int i = 0; i < graph[node].size(); i++) {
            int next = graph[node][i];
            if (color[next] == 0) {
                color[next] = color[node];
                class_capacity_left[color[node]]--;
                not_colored_nodes--;
                q[0].push(next);
                if (nth_choice + 1 < q.size()) q[nth_choice + 1].push(node);
                break;
            }
        }
    }

    for (auto x : q) {
        while (!x.empty()) {
            x.pop();
        }
    }

    return;
}

int get_most_capacios_class(std::vector<int>& class_capacity_left, int N_CLASSES) {
    int id = 1;
    for (int i = 2; i <= N_CLASSES; i++) {
        if (class_capacity_left[id] < class_capacity_left[i]) {
            id = i;
        }
    }
    assert(class_capacity_left[id] > 0);
    return id;
}

std::pair<int, int> get_node(std::vector<int>& color, std::vector<std::queue<int> >& q,
                             std::vector<int>& class_capacity_left, int N_CLASSES,
                             int& not_colored_nodes) {
    // there are nodes in the std::queue?
    //  	-YES: ok, return the first one
    // 		-NO: select a node from the ones not colored yet
    int index = 0;
    while (index < q.size() && q[index].empty()) {
        index++;
    }
    if (index < q.size()) {
        int node = q[index].front();
        q[index].pop();
        return std::make_pair(node, index);
    }

    int next = rand() % not_colored_nodes;
    for (int i = 0, nth = 0; i < color.size(); i++) {
        if (color[i] == 0) {
            if (next == nth) {
                color[i] = get_most_capacios_class(class_capacity_left, N_CLASSES);
                not_colored_nodes--;
                class_capacity_left[color[i]]--;
                return std::make_pair(i, 0);
            } else {
                nth++;
            }
        }
    }
    return std::make_pair(-1, -1);
}

void assign_color_DSA(std::vector<student>& students_data, std::vector<int>& color,
                      std::vector<std::queue<int> >& q, std::vector<int>& class_capacity_left,
                      std::string& sections) {
    // IMPORTANT: DSA's class preference must be inserted by you (if not specified by him)
    for (int i = 0; i < students_data.size(); i++) {
        if (students_data[i].DSA) {
            color[students_data[i].id] = (int)(students_data[i].classes[0][0] - sections[0]) + 1;
            class_capacity_left[color[students_data[i].id]]--;
            q[0].push(students_data[i].id);
        }
    }
}

void assign_color_class_preference(std::vector<student>& students_data, std::vector<int>& color,
                                   std::vector<std::queue<int> >& q,
                                   std::vector<int>& class_capacity_left, std::string& sections) {
    for (int i = 0; i < students_data.size(); i++) {
        if (students_data[i].preferences.empty()) continue;
        if (students_data[i].preferences[0] == "CLASSE") {
            // CONTROLLA CHE LA CLASSE ABBIA ANCORA POSTO!!!!!!!!!!!!!!!!!!
            color[students_data[i].id] = (int)(students_data[i].classes[0][0] - sections[0]) + 1;
            class_capacity_left[color[students_data[i].id]]--;
            q[0].push(students_data[i].id);
        }
    }
}

void assign_color_repeating(std::vector<student>& students_data, std::vector<int>& color,
                            std::vector<std::queue<int> >& q, std::vector<int>& class_capacity_left,
                            std::string& sections, int N_CLASSES) {
    // distribution of repeating students
    std::vector<int> repeating_students_to_be_placed;
    int cont_repeating_students_assigned[N_CLASSES + 1] = {0};
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >,
                        std::greater<std::pair<int, int> > >
        class_capacity_repeating_students;
    for (int i = 0; i < students_data.size(); i++) {
        if (students_data[i].repeating) {
            if (color[i] == 0) {
                repeating_students_to_be_placed.push_back(i);
            } else {
                cont_repeating_students_assigned[color[i]]++;
            }
        }
    }

    // in this way, every class has almost the same number of repeating students, taking into
    // account the number of rep students already assigned until now
    for (int i = 1; i <= N_CLASSES; i++) {
        class_capacity_repeating_students.push(
            std::make_pair(cont_repeating_students_assigned[i], i));
    }
    shuffle(repeating_students_to_be_placed.begin(), repeating_students_to_be_placed.end(),
            std::default_random_engine(rand()));
    for (int i = 0; i < repeating_students_to_be_placed.size(); i++) {
        std::pair<int, int> next_class = class_capacity_repeating_students.top();
        class_capacity_repeating_students.pop();
        color[repeating_students_to_be_placed[i]] = next_class.second;
        class_capacity_left[next_class.first]--;
        next_class.first++;
        class_capacity_repeating_students.push(next_class);
        q[0].push(repeating_students_to_be_placed[i]);
    }
}
