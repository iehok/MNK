import os
import random
import sys

import pandas as pd
from sklearn.cluster import KMeans

from utils import load_params

params = load_params()
POP_SIZE, EPOCH, DEPTH, WIDTH, TIMES, N, M, K = params.values()
K_STR = ''.join([str(k) for k in K])


def encode2(x):
    res = []
    for i in range(N):
        res.append(x // pow(2, N - i - 1))
        x = x % pow(2, N - i - 1)
    return res


if __name__ == '__main__':
    truePareto_path = f'../data/true_pareto/N{N}/K{K_STR}/true_pareto.csv'
    checkpoint_path = f'../data/checkpoint/N{N}/K{K_STR}/d{DEPTH}w{WIDTH}t{TIMES}/checkpoint.csv'

    truePareto_df = pd.read_csv(truePareto_path, header=None)
    checkpoint_df = pd.read_csv(checkpoint_path, header=None)

    # checkpoint_df = checkpoint_df[~(checkpoint_df[3] > 1)]  # remove not-rank_1 solutions

    print('truePareto_df')
    print(truePareto_df.head())

    print('checkpoint_df')
    print(checkpoint_df.head())

    # |-----------------------------------------------------------
    # | Evaluate the performance in the objective function space |
    # |-----------------------------------------------------------
    n_truePareto_solutions = len(truePareto_df)
    correct = 0
    for x in checkpoint_df.itertuples(index=False):
        for y in truePareto_df.itertuples(index=False):
            same = True
            for i in range(M):
                if x[i] != y[i]:
                    same = False
                    break
            if same:
                correct += 1
    print('score_obj: {}/{}({:.1f}%)'.format(correct, n_truePareto_solutions, correct / n_truePareto_solutions * 100))

    # |--------------------------------------------------------
    # | Evaluate the performance in the design variable space |
    # |--------------------------------------------------------
    SEED = 277
    random.seed(SEED)
    n_clusters_map = {32: {'88': 18}, 40: {'88': 18, '888': 200}}
    n_clusters = n_clusters_map[N][K_STR]
    truePareto_df[M + 1] = truePareto_df[M].apply(encode2)
    pred = KMeans(n_clusters=n_clusters, random_state=SEED, n_init=10).fit_predict(truePareto_df[M + 1].to_list())
    truePareto_df = pd.concat([truePareto_df, pd.DataFrame(pred, columns=[M + 2])], axis=1)
    correct_clusters = [0] * n_clusters
    for x in checkpoint_df.itertuples(index=False):
        for y in truePareto_df.itertuples(index=False):
            same = True
            for i in range(M):
                if x[i] != y[i]:
                    same = False
                    break
            if same:
                correct_clusters[y[M + 2]] = 1
    correct = sum(correct_clusters)
    print('score_design: {}/{}({:.1f}%)'.format(correct, n_clusters, correct / n_clusters * 100))
