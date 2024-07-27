#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include <iostream>
#include <vector>

#include "config.h"

using namespace std;

class Individual {
   public:
    int rank;                  //
    int dominate;              // the number of individuals that this individual dominates
    double crowding_distance;  //
    vector<int> dominated;     // indices of individuals that this individual is dominated
    vector<int> chromosome;    //
    vector<double> fitness;    //
    Individual();
    void echo();
};

Individual::Individual() {
    chromosome.resize(N);
    fitness.resize(M);
}

void Individual::echo() {
    for (int i = 0; i < chromosome.size(); i++) printf("%d", chromosome[i]);
    for (int i = 0; i < fitness.size(); i++) printf(" %.6lf", fitness[i]);
    printf(" %d\n", rank);
}

#endif
