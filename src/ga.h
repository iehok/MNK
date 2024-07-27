#ifndef GA_H_
#define GA_H_

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "config.h"
#include "individual.h"
#include "nkmodel.h"
#include "nsga2.h"
#include "rand.h"
#include "utils.h"

class GA {
   public:
    template <class T>
    static void multi_point_crossover(vector<T> p1, vector<T> p2, vector<T> &q1, vector<T> &q2, int point);
    static void uniform_crossover(const vector<int> &p1, const vector<int> &p2, vector<int> &q1, vector<int> &q2, const double CX_PB);
    static void dMS(const Individual &p1, const Individual &p2, vector<Individual> &q, vector<NKmodel> &nk, bool is_crossover);
    static void bit_flip_mutation(vector<int> &a, const double MU_PB);
};

template <class T>
void GA::multi_point_crossover(vector<T> p1, vector<T> p2, vector<T> &q1, vector<T> &q2, int point) {
    vector<int> pos(p1.size());
    for (int i = 0; i < pos.size(); i++) pos[i] = i;
    random_shuffle(pos.begin(), pos.end(), genrand_int);
    pos.resize(point);
    pos.push_back(0);
    pos.push_back(p1.size());
    sort(pos.begin(), pos.end());
    for (int i = 0; i < pos.size() - 1; i++) {
        for (int j = pos[i]; j < pos[i + 1]; j++) {
            q1[j] = (i % 2 == 0) ? p1[j] : p2[j];
            q2[j] = (i % 2 == 0) ? p2[j] : p1[j];
        }
    }
}

void GA::uniform_crossover(const vector<int> &p1, const vector<int> &p2, vector<int> &q1, vector<int> &q2, const double CX_PB) {
    for (int i = 0; i < p1.size(); i++) {
        double r = genrand_real2();
        q1[i] = (r <= CX_PB) ? p1[i] : p2[i];
        q2[i] = (r <= CX_PB) ? p2[i] : p1[i];
    }
}

vector<int> get_pos(const vector<int> &a, const vector<int> &b, bool is_crossover = true) {
    vector<int> pos;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] == b[i] && !is_crossover) pos.push_back(i);
        if (a[i] != b[i] && is_crossover) pos.push_back(i);
    }
    return pos;
}

void GA::dMS(const Individual &p1, const Individual &p2, vector<Individual> &q, vector<NKmodel> &nk, bool is_crossover = true) {
    Individual best = p1;
    q[0] = best;
    int dis_best_p2 = get_pos(best.chromosome, p2.chromosome, is_crossover).size();
    int step_size = q.size();
    int diff_size[2] = {dis_best_p2 / (step_size - 1), (int)ceil((double)dis_best_p2 / (step_size - 1))};
    for (int i = 1; i < q.size(); i++) {
        vector<int> pos = get_pos(best.chromosome, p2.chromosome, is_crossover);
        dis_best_p2 = pos.size();
        int nghb_size = 16;
        // int nghb_size = max(1, (int)ceil((double)(dis_best_p2 * dis_best_p2) / (best.chromosome.size() * best.chromosome.size()) * POP_SIZE / 2 * (2 * 2)));
        vector<Individual> nghb(nghb_size);
        for (int j = 0; j < nghb_size; j++) {
            Individual tmp = best;
            random_shuffle(pos.begin(), pos.end(), genrand_int);
            int diff = diff_size[genrand_int(2)];
            for (int k = 0; k < min(dis_best_p2, diff); k++) tmp.chromosome[pos[k]] = 1 - tmp.chromosome[pos[k]];
            for (int k = 0; k < tmp.fitness.size(); k++) tmp.fitness[k] = nk[k].evaluate(tmp.chromosome);
            nghb[j] = tmp;
        }
        vector<Individual> rank1;
        NSGA2::merge(nghb, rank1);
        best = rank1[genrand_int(rank1.size())];
        q[i] = best;
    }
}

void GA::bit_flip_mutation(vector<int> &a, const double MU_PB) {
    for (int i = 0; i < a.size(); i++)
        if (genrand_real2() <= MU_PB) a[i] = 1 - a[i];
}

#endif
