#ifndef NSGA2_H_
#define NSGA2_H_

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "individual.h"
#include "rand.h"
#include "utils.h"

using namespace std;

class NSGA2 {
   public:
    static void fast_nondominated_sort(vector<vector<Individual>> &a, vector<Individual> &b);
    static void merge(vector<Individual> &a, vector<Individual> &b);
    static void crowding_distance_assignment(vector<Individual> &a);
    static void ranking_selection(vector<vector<Individual>> &a, vector<Individual> &b);
};

bool is_dominated(const vector<double> &a, const vector<double> &b) {
    bool same = true;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) same = false;
        if (a[i] > b[i]) return false;
    }
    return !same ? true : false;
}

void NSGA2::fast_nondominated_sort(vector<vector<Individual>> &a, vector<Individual> &b) {
    // initialization
    a.resize(b.size());
    for (int i = 0; i < a.size(); i++) a[i].resize(0);
    for (int i = 0; i < b.size(); i++) {
        b[i].dominated.resize(0);
        b[i].dominate = 0;
    }
    // search the first front
    for (int i = 0; i < b.size(); i++) {
        for (int j = 0; j < b.size(); j++) {
            if (i != j) {
                if (is_dominated(b[i].fitness, b[j].fitness))
                    b[i].dominated.push_back(j);
                else if (is_dominated(b[j].fitness, b[i].fitness))
                    b[i].dominate++;
            }
        }
        if (b[i].dominate == 0) {
            b[i].rank = 1;
            a[0].push_back(b[i]);
        }
    }
    // search other fronts
    for (int i = 0; i < a.size() && !a[i].empty(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            for (int k = 0; k < a[i][j].dominated.size(); k++) {
                b[a[i][j].dominated[k]].dominate--;
                if (b[a[i][j].dominated[k]].dominate == 0) {
                    b[a[i][j].dominated[k]].rank = i + 2;
                    a[i + 1].push_back(b[a[i][j].dominated[k]]);
                }
            }
        }
    }
    b.resize(0);
}

void NSGA2::merge(vector<Individual> &a, vector<Individual> &b) {
    // initialization
    a.resize(a.size() + b.size());
    for (int i = 0; i < b.size(); i++) a[a.size() - b.size() + i] = b[i];
    b.resize(0);
    for (int i = 0; i < a.size(); i++) {
        a[i].dominated.resize(0);
        a[i].dominate = 0;
    }
    // search the first front
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a.size(); j++) {
            if (i != j) {
                if (is_dominated(a[i].fitness, a[j].fitness))
                    a[i].dominated.push_back(j);
                else if (is_dominated(a[j].fitness, a[i].fitness))
                    a[i].dominate++;
            }
        }
        if (a[i].dominate == 0) {
            a[i].rank = 1;
            b.push_back(a[i]);
        }
    }
    a.resize(0);
}

void NSGA2::crowding_distance_assignment(vector<Individual> &a) {
    int n = a.size();
    for (int i = 0; i < n; i++) a[i].crowding_distance = 0;
    for (int i = 0; i < a[0].fitness.size(); i++) {
        sort(a.begin(), a.end(), [=](const Individual &x, const Individual &y) { return x.fitness[i] < y.fitness[i]; });
        a[0].crowding_distance = a[n - 1].crowding_distance = 1e6;
        for (int j = 1; j < n - 1; j++) a[j].crowding_distance += (a[j + 1].fitness[i] - a[j - 1].fitness[i]) / (a[n - 1].fitness[i] - a[0].fitness[i]);
    }
}

void NSGA2::ranking_selection(vector<vector<Individual>> &a, vector<Individual> &b) {
    int c = 0;
    for (int i = 0; i < a.size(); i++) {
        crowding_distance_assignment(a[i]);
        if (c + a[i].size() > b.size()) sort(a[i].begin(), a[i].end(), [](const Individual &x, const Individual &y) { return x.crowding_distance > y.crowding_distance; });
        for (int j = 0; j < a[i].size(); j++) {
            b[c++] = a[i][j];
            if (c == b.size()) return;
        }
    }
}

#endif
