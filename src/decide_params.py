import os
import random
import sys

import pandas as pd
from sklearn.cluster import KMeans


def encode2(x):
    res = []
    for i in range(N):
        res.append(x // pow(2, N - i - 1))
        x = x % pow(2, N - i - 1)
    return res


def hamming_distance(a, b):
    res = 0
    for i in range(N):
        if a[i] != b[i]:
            res += 1
    return res


def get_dis_max(truePareto_df, seed, n_clusters):
    random.seed(seed)
    pred = KMeans(n_clusters=n_clusters, random_state=seed, n_init=10).fit_predict(truePareto_df[M + 1].to_list())

    truePareto_df = pd.concat([truePareto_df, pd.DataFrame(pred, columns=[M + 2])], axis=1)
    truePareto_df = truePareto_df.sort_values(by=[M + 2]).reset_index(drop=True)

    cluster_df = truePareto_df[M + 2].value_counts().sort_index().reset_index()
    cluster_df.columns = ['cluster_id', 'cluster_size']

    pos = 0
    dis_max = 0
    for cluster_size in cluster_df['cluster_size']:
        for i in range(pos, pos + cluster_size - 1):
            for j in range(i + 1, pos + cluster_size):
                dis = hamming_distance(truePareto_df[M + 1].to_list()[i], truePareto_df[M + 1].to_list()[j])
                dis_max = max(dis_max, dis)
        pos += cluster_size

    return dis_max


if __name__ == '__main__':
    # -------------------------------------------------------------------------------
    # | Decide the parameters (the number of cluster and seed) used in exeperiments |
    # -------------------------------------------------------------------------------
    for seed in range(200000):
        # ---------------------------
        # | (M, N, K) = (2, 40, 88) |
        # ---------------------------
        M = 2
        N = 40
        K = 88
        n_clusters = 18
        truePareto_path = f'../data/true_pareto/N{N}/K{K}/true_pareto.csv'
        truePareto_df = pd.read_csv(truePareto_path, header=None)
        truePareto_df[M + 1] = truePareto_df[M].apply(encode2)

        dis_max = get_dis_max(truePareto_df, seed, n_clusters)
        if dis_max > 10:
            continue

        # ----------------------------
        # | (M, N, K) = (3, 40, 888) |
        # ----------------------------
        M = 3
        N = 40
        K = 888
        n_clusters = 200
        truePareto_path = f'../data/true_pareto/N{N}/K{K}/true_pareto.csv'
        truePareto_df = pd.read_csv(truePareto_path, header=None)
        truePareto_df[M + 1] = truePareto_df[M].apply(encode2)

        dis_max = get_dis_max(truePareto_df, seed, n_clusters)
        if dis_max > 10:
            continue

        print(seed, dis_max)
        break

    # -----------------------------------------------------------------------
    # | Finally, we decide the parameters used in experiments as follows:   |
    # | (M, N, K) = (2, 40, 88)  -> seed is 277, n_clusters is 18,          |
    # | (M, N, K) = (3, 40, 888) -> seed is 277, n_clusters is 200.         |
    # -----------------------------------------------------------------------
