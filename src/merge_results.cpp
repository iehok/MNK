#include <iostream>
#include <vector>

#include "config.h"
#include "individual.h"
#include "nkmodel.h"
#include "nsga2.h"
#include "population.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) return 0;

    // prepare NK problems
    rand_global = Rand(1);
    NKmodel nk1(N, K1);
    NKmodel nk2(N, K2);
    NKmodel nk3(N, K3);
    vector<NKmodel> nk = {nk1, nk2, nk3};
    nk.erase(nk.begin() + M, nk.end());

    vector<Population> P(atoi(argv[1]));

    string K = "";
    for (int i = 0; i < nk.size(); i++) K += to_string((int)log2(nk[i].rand_table[0].size()) - 1);

    for (int i = 0; i < P.size(); i++) {
        string item = format("../data/checkpoint/N%d/K%s/d%dw%dt%d/checkpoint-%d.csv", nk[0].rand_table.size(), K.c_str(), DEPTH, WIDTH, TIMES, i);
        P[i].read_csv(item, true);
    }

    // slow
    // for (int i = 1; i < P.size(); i++) NSGA2::merge(P[i].pop, P[0].pop);

    // fast (argv[1] must be a power of two)
    for (int i = 0; i < (int)log2(P.size()); i++)
        for (int j = 0; j < P.size(); j += (1 << (i + 1))) NSGA2::merge(P[j + (1 << i)].pop, P[j].pop);

    P[0].remove_same_solutions();

    P[0].head();

    vector<string> items = {format("../data/checkpoint/N%d/K%s/d%dw%dt%d/checkpoint.csv", nk[0].rand_table.size(), K.c_str(), DEPTH, WIDTH, TIMES)};
    P[0].to_csv(items, true);

    return 0;
}
