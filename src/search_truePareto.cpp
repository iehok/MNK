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

    Population P, Q;
    Individual tmp;

    int OFFSET_LEN = 2;  // feel free to change this value
    int n = N - OFFSET_LEN;
    long long OFFSET_VAL = atoll(argv[1]);
    long long s = OFFSET_VAL;

    for (int i = OFFSET_LEN - 1; i >= 0; i--) {
        tmp.chromosome[n + i] = s / (1LL << i);
        s %= (1LL << i);
    }

    for (long long i = 0; i < (1LL << n); i++) {
        s = i;
        for (int j = n - 1; j >= 0; j--) {
            tmp.chromosome[j] = s / (1LL << j);
            s %= (1LL << j);
        }
        for (int j = 0; j < tmp.fitness.size(); j++) tmp.fitness[j] = nk[j].evaluate(tmp.chromosome);
        P.pop.push_back(tmp);
        if ((i + 1) % (1LL << 10) == 0) NSGA2::merge(P.pop, Q.pop);
    }

    string K = "";
    for (int i = 0; i < nk.size(); i++) K += to_string((int)log2(nk[i].rand_table[0].size()) - 1);

    vector<string> items = {format("../data/true_pareto/N%d", N), format("K%s", K.c_str()), format("true_pareto_%d.csv", OFFSET_VAL)};

    Q.to_csv(items);

    return 0;
}
