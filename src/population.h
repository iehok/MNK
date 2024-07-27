#ifndef POPULATION_H_
#define POPULATION_H_

#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "individual.h"
#include "rand.h"
#include "utils.h"

using namespace std;

class Population {
   public:
    vector<Individual> pop;
    Population();
    Population(int pop_size);
    void head();
    void remove_same_solutions();
    void read_csv(const string &input_file, bool with_rank);
    void to_csv(const vector<string> &items, bool with_rank);
};

Population::Population() {}

Population::Population(int pop_size) { pop.resize(pop_size); }

void Population::head() {
    for (int i = 0; i < 5; i++) pop[i].echo();
    printf("...\n");
}

void Population::remove_same_solutions() {
    vector<pair<long long, int>> pairs(pop.size());
    for (int i = 0; i < pairs.size(); i++) pairs[i] = make_pair(decode(pop[i].chromosome), i);
    sort(pairs.begin(), pairs.end());
    for (int i = 1; i < pairs.size();) {
        if (pairs[i - 1].first == pairs[i].first)
            pairs.erase(pairs.begin() + i);
        else
            i++;
    }
    sort(pairs.begin(), pairs.end(), [](pair<long long, int> x, pair<long long, int> y) { return (x.second != y.second) ? (x.second < y.second) : (x.first < y.first); });
    for (int i = 0; i < pairs.size(); i++) pop[i] = pop[pairs[i].second];
    pop.resize(pairs.size());
}

void Population::read_csv(const string &input_file, bool with_rank = false) {
    FILE *fp = fopen(input_file.c_str(), "r");
    if (fp == NULL) {
        printf("%s was not found.\n", input_file.c_str());
        return;
    }
    Individual tmp;
    while (fscanf(fp, "%lf,", &tmp.fitness[0]) != EOF) {
        for (int i = 1; i < tmp.fitness.size(); i++) int c = fscanf(fp, "%lf,", &tmp.fitness[i]);
        long long dec;
        int c = fscanf(fp, "%lld,", &dec);
        tmp.chromosome = encode(dec, tmp.chromosome.size());
        int rank = fscanf(fp, "%d,", &tmp.rank);
        pop.push_back(tmp);
    }
    printf("%s was found.\n", input_file.c_str());
    fclose(fp);
}

void Population::to_csv(const vector<string> &items, bool with_rank = false) {
    // make the directory
    string ofile;
    for (int i = 0; i < items.size() - 1; i++) {
        ofile += items[i];
        mkdir(ofile.c_str(), S_IRWXU | S_IRGRP | S_IXGRP);
        ofile += "/";
    }
    ofile += items[items.size() - 1];
    // save this population to the .csv file.
    FILE *fp = fopen(ofile.c_str(), "w");
    for (int i = 0; i < pop.size(); i++) {
        for (int j = 0; j < pop[i].fitness.size(); j++) fprintf(fp, "%.6lf,", pop[i].fitness[j]);
        fprintf(fp, "%lld", decode(pop[i].chromosome));
        if (with_rank) fprintf(fp, ",%d", pop[i].rank);
        fprintf(fp, "\n");
    }
    fclose(fp);
}

#endif
