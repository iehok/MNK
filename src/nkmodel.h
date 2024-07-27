#ifndef NKMODEL_H_
#define NKMODEL_H_

#include <cmath>
#include <iostream>
#include <vector>

#include "rand.h"

class NKmodel {
   public:
    vector<vector<double>> rand_table;
    NKmodel();
    NKmodel(int n, int k);
    double evaluate(const vector<int> &a);
};

NKmodel::NKmodel() {}

NKmodel::NKmodel(int n, int k) {
    rand_table.resize(n);
    for (int i = 0; i < rand_table.size(); i++) {
        rand_table[i].resize(1 << (k + 1));
        for (int j = 0; j < rand_table[i].size(); j++) rand_table[i][j] = genrand_real2();
    }
}

double NKmodel::evaluate(const vector<int> &a) {
    int n = a.size(), k = log2(rand_table[0].size()) - 1;
    double res = 0;
    for (int i = 0; i < n; i++) {
        int sum = 0, t = k;
        sum += (1 << t--) * a[i];
        for (int j = 1; j <= k; j++) sum += (1 << t--) * a[(i + j) % n];
        res += rand_table[i][sum];
    }
    res /= n;
    return res;
}

#endif
