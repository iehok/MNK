#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#include "config.h"
#include "floor.h"
#include "ga.h"
#include "individual.h"
#include "nkmodel.h"
#include "nsga2.h"
#include "population.h"
#include "rand.h"
#include "room.h"
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

    // initialize building
    // building: all division patterns used in this search
    // floor   : division patterns at each depth
    // room    : a division pattern
    vector<Floor> building(DEPTH + 1);
    Room empty_division_pattern;
    building[0].insert(empty_division_pattern);

    int current_trial = 0;

    set<long long> unique_solutions;
    vector<long long> unique_solutions_size;

    current_time();

    for (int depth = 0; depth < DEPTH; depth++) {

        for (int width = 0; width < min((int)building[depth].floor.size(), WIDTH); width++) {

            printf("building[%d].floor[%d].u: ", depth, width);
            building[depth].floor[width].echo();

            for (int times = 0; times < TIMES; times++) {

                fprintf(stderr, "\r[ %4d / %4d ]", times + 1, TIMES);

                rand_global = Rand(current_trial + 1);

                // initialize populations
                // P: parents
                // Q: children
                Population P(POP_SIZE), Q(POP_SIZE);

                // assign a random value to each locus
                for (int i = 0; i < P.pop.size(); i++)
                    for (int j = 0; j < P.pop[i].chromosome.size(); j++) P.pop[i].chromosome[j] = genrand_int(2);

                // change values of the loci that exist in the division pattern
                for (int i = 0; i < P.pop.size(); i++)
                    for (decltype(building[depth].floor[width].u)::iterator itr = building[depth].floor[width].u.begin(); itr != building[depth].floor[width].u.end(); itr++)
                        P.pop[i].chromosome[(*itr).first] = (*itr).second;

                // evaluate P
                for (int i = 0; i < P.pop.size(); i++)
                    for (int j = 0; j < P.pop[i].fitness.size(); j++) P.pop[i].fitness[j] = nk[j].evaluate(P.pop[i].chromosome);

                for (int epoch = 0; epoch < EPOCH; epoch++) {

                    random_shuffle(P.pop.begin(), P.pop.end(), genrand_int);

                    // crossover
                    for (int i = 0; i < P.pop.size(); i += 2) GA::multi_point_crossover(P.pop[i + 0].chromosome, P.pop[i + 1].chromosome, Q.pop[i + 0].chromosome, Q.pop[i + 1].chromosome, 2);

                    // mutation
                    for (int i = 0; i < Q.pop.size(); i++) GA::bit_flip_mutation(Q.pop[i].chromosome, 1.0 / (Q.pop[i].chromosome.size() - building[depth].floor[width].u.size()));

                    // change values of the loci that exist in the division pattern
                    for (int i = 0; i < Q.pop.size(); i++)
                        for (decltype(building[depth].floor[width].u)::iterator itr = building[depth].floor[width].u.begin(); itr != building[depth].floor[width].u.end(); itr++)
                            Q.pop[i].chromosome[(*itr).first] = (*itr).second;

                    // evaluate Q
                    for (int i = 0; i < Q.pop.size(); i++)
                        for (int j = 0; j < Q.pop[i].fitness.size(); j++) Q.pop[i].fitness[j] = nk[j].evaluate(Q.pop[i].chromosome);

                    // make R by concatenating P and Q
                    Population R(P.pop.size() + Q.pop.size());
                    int c = 0;
                    for (int i = 0; i < P.pop.size(); i++) R.pop[c++] = P.pop[i];
                    for (int i = 0; i < Q.pop.size(); i++) R.pop[c++] = Q.pop[i];

                    // remove the same solutions in R to keep diversity
                    R.remove_same_solutions();

                    vector<vector<Individual>> F;

                    NSGA2::fast_nondominated_sort(F, R.pop);

                    NSGA2::ranking_selection(F, P.pop);
                }

                // identify unique solutions
                long long unique_solutions_size_prev = unique_solutions.size();
                for (int i = 0; i < P.pop.size(); i++) unique_solutions.insert(decode(P.pop[i].chromosome));
                unique_solutions_size.push_back(unique_solutions.size() - unique_solutions_size_prev);

                // save the result at this trial
                string K = "";
                for (int i = 0; i < nk.size(); i++) K += to_string((int)log2(nk[i].rand_table[0].size()) - 1);
                vector<string> items = {format("../data/checkpoint/N%d", nk[0].rand_table.size()),
                                        format("K%s", K.c_str()),
                                        format("d%dw%dt%d", DEPTH, WIDTH, TIMES),
                                        format("checkpoint-%d.csv", current_trial++)};
                P.to_csv(items, true);

                // get candidates of next division pattern
                for (int i = 0; i < P.pop[0].chromosome.size(); i++) {
                    if ((building[depth].floor[width].u.find({i, 0}) != building[depth].floor[width].u.end()) || (building[depth].floor[width].u.find({i, 1}) != building[depth].floor[width].u.end())) continue;
                    bool same[2] = {false};
                    for (int j = 0; j < P.pop.size() - 1; j++) {
                        if (same[P.pop[j].chromosome[i]]) continue;
                        for (int k = j + 1; k < P.pop.size(); k++) {
                            if ((hamming_distance(P.pop[j].chromosome, P.pop[k].chromosome) <= 10) && (P.pop[j].chromosome[i] == P.pop[k].chromosome[i])) {
                                same[P.pop[j].chromosome[i]] = true;
                                break;
                            }
                        }
                    }
                    if (same[0] != same[1]) {
                        Room tmp = building[depth].floor[width];
                        tmp.u.insert({i, !same[0] ? 0 : 1});
                        tmp.score = 1;
                        building[depth + 1].insert(tmp);
                    }
                }
            }
            printf("\n");
        }

        // sort the next floor
        random_shuffle(building[depth + 1].floor.begin(), building[depth + 1].floor.end(), genrand_int);
        sort(building[depth + 1].floor.begin(), building[depth + 1].floor.end());

        for (int i = 0; i < building[depth + 1].floor.size(); i++) printf("%d ", building[depth + 1].floor[i].score);
        printf("\n");
    }

    // make the directory
    string K = "";
    for (int i = 0; i < nk.size(); i++) K += to_string((int)log2(nk[i].rand_table[0].size()) - 1);
    vector<string> items = {format("../data/unique_solutions/N%d", nk[0].rand_table.size()),
                            format("K%s", K.c_str()),
                            format("d%dw%dt%d", DEPTH, WIDTH, TIMES),
                            "unique_solutions.csv"};
    string ofile;
    for (int i = 0; i < items.size() - 1; i++) {
        ofile += items[i];
        mkdir(ofile.c_str(), S_IRWXU | S_IRGRP | S_IXGRP);
        ofile += "/";
    }
    ofile += items[items.size() - 1];

    // save the number of unique solutions for all trials to the .csv file
    FILE *fp = fopen(ofile.c_str(), "w");
    for (int i = 0; i < unique_solutions_size.size(); i++) fprintf(fp, "%d, %lld\n", i + 1, unique_solutions_size[i]);
    fclose(fp);

    current_time();

    return 0;
}
