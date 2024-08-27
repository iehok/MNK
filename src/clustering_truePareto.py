import os
import random
import sys

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
from sklearn.cluster import KMeans

from utils import load_params, make_dir

params = load_params()
POP_SIZE, EPOCH, DEPTH, WIDTH, TIMES, N, M, K = params.values()
K_STR = ''.join([str(k) for k in K])


def encode2(x):
    res = []
    for i in range(N):
        res.append(x // pow(2, N - i - 1))
        x = x % pow(2, N - i - 1)
    return res


def plot(truePareto_df, output_path):
    sns.set_style('whitegrid')
    ax = sns.scatterplot(data=truePareto_df, x=0, y=1, hue=(M + 2), marker='+', s=150, linewidth=1.5, legend=False, palette='tab20')
    plt.xlabel('f1', fontsize=12)
    plt.ylabel('f2', fontsize=12)
    plt.tick_params(labelsize=12)
    for i in range(len(truePareto_df)):
        txt = chr(945 + truePareto_df[M + 2][i])
        if txt in {chr(946), chr(952), chr(959)}:
            ax.annotate(txt, (truePareto_df[0][i] - 0.007, truePareto_df[1][i] - 0.008), size=10.0)
        else:
            ax.annotate(txt, (truePareto_df[0][i] + 0.001, truePareto_df[1][i] + 0.002), size=10.0)
    plt.savefig(output_path, dpi=500, bbox_inches='tight')
    print(f'The figure is saved to {output_path}.')


if __name__ == '__main__':
    args = sys.argv
    if len(args) != 2:
        sys.exit()

    truePareto_path = f'../data/true_pareto/N{N}/K{K_STR}/true_pareto.csv'

    truePareto_df = pd.read_csv(truePareto_path, header=None)

    truePareto_df[M + 1] = truePareto_df[M].apply(encode2)

    for i, x in enumerate(truePareto_df[M + 1]):
        print('{:<2} {}'.format(i, x))

    SEED = 277
    random.seed(SEED)
    pred = KMeans(n_clusters=int(args[1]), random_state=SEED, n_init=10).fit_predict(truePareto_df[M + 1].to_list())
    truePareto_df = pd.concat([truePareto_df, pd.DataFrame(pred, columns=[M + 2])], axis=1)

    print('truePareto_df')
    print(truePareto_df)

    output_dir = f'../data/result/N{N}/K{K_STR}'
    output_path = os.path.join(output_dir, 'true_pareto-clusters.png')
    make_dir(output_dir)
    plot(truePareto_df, output_path)
