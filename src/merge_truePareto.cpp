#include <iostream>
#include <vector>

#include "config.h"
#include "individual.h"
#include "nkmodel.h"
#include "nsga2.h"
#include "population.h"
#include "utils.h"

using namespace std;

int main() {
    // prepare NK problems
    rand_global = Rand(1);
    NKmodel nk1(N, K1);
    NKmodel nk2(N, K2);
    NKmodel nk3(N, K3);
    vector<NKmodel> nk = {nk1, nk2, nk3};
    nk.erase(nk.begin() + M, nk.end());

    int OFFSET_LEN = 2;  // feel free to change this value

    vector<Population> P(1 << OFFSET_LEN);

    string K = "";
    for (int i = 0; i < M; i++) K += to_string((int)log2(nk[i].rand_table[0].size()) - 1);

    for (int i = 0; i < P.size(); i++) P[i].read_csv(format("../data/true_pareto/N%d/K%s/true_pareto_%d.csv", N, K.c_str(), i));

    for (int i = 0; i < OFFSET_LEN; i++)
        for (int j = 0; j < P.size(); j += (1 << (i + 1))) NSGA2::merge(P[j + (1 << i)].pop, P[j].pop);

    P[0].head();

    vector<string> items = {format("../data/true_pareto/N%d", N), format("K%s", K.c_str()), "true_pareto.csv"};

    P[0].to_csv(items);

    return 0;
}
