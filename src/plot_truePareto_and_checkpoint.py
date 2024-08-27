import os
import sys

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

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


def plot(truePareto_df, checkpoint_df, output_path):
    sns.set_style('whitegrid')
    ax = truePareto_df.plot.scatter(x=0, y=1, label='true pareto', c='r', marker='+', s=150, linewidths=1.5, zorder=3)
    checkpoint_df.plot.scatter(x=0, y=1, label='rank1', c='None', marker=',', edgecolors='g', s=150, linewidths=1.5, zorder=2, ax=ax)
    plt.xlabel('f1', fontsize=12)
    plt.ylabel('f2', fontsize=12)
    plt.tick_params(labelsize=12)
    plt.legend(fontsize=12)
    plt.grid(zorder=1)
    plt.savefig(output_path, dpi=500, bbox_inches='tight')
    print(f'The figure is saved to {output_path}.')


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

    if M == 2:
        output_dir = f'../data/result/N{N}/K{K_STR}/d{DEPTH}w{WIDTH}t{TIMES}'
        output_path = os.path.join(output_dir, 'checkpoint.png')
        make_dir(output_dir)
        plot(truePareto_df, checkpoint_df, output_path)
