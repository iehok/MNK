import os
import sys

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

from utils import load_params

if __name__ == '__main__':
    params = load_params()
    POP_SIZE, EPOCH, DEPTH, WIDTH, TIMES, N, M, K = params.values()
    K_STR = ''.join([str(k) for k in K])

    unique_solutions_path_map = {
        '(1, 1, 100)': f'../data/unique_solutions/N{N}/K{K_STR}/d1w1t100/unique_solutions.csv',
        '(2, 4, 20)': f'../data/unique_solutions/N{N}/K{K_STR}/d2w4t20/unique_solutions.csv',
        '(4, 3, 10)': f'../data/unique_solutions/N{N}/K{K_STR}/d4w3t10/unique_solutions.csv',
        '(4, 8, 4)': f'../data/unique_solutions/N{N}/K{K_STR}/d4w8t4/unique_solutions.csv',
        '(7, 4, 4)': f'../data/unique_solutions/N{N}/K{K_STR}/d7w4t4/unique_solutions.csv',
    }

    sns.set_style('whitegrid')
    fig, ax = plt.subplots(figsize=(8, 5))
    for triplet, path in unique_solutions_path_map.items():
        df = pd.read_csv(path, header=None)
        for i in range(1, len(df[1])):
            df.loc[i, 1] = df.loc[i, 1] + df.loc[i - 1, 1]
        ax.plot(df[0], df[1] / 10000, linewidth=5.0, alpha=0.75, label=triplet)
    ax.plot(range(1, 101), [i / 100 for i in range(1, 101)], linewidth=5.0, alpha=0.75, label='maximum', linestyle='--', color='tab:gray')
    ax.legend()
    output_path = f'../data/result/N{N}/K{K_STR}/unique_solutions.png'
    plt.xlabel('#trials', fontsize=16)
    plt.ylabel(r'#unique solutions (${\times}10^4$)', fontsize=16)
    plt.tick_params(labelsize=16)
    plt.legend(fontsize=16)
    plt.savefig(output_path, dpi=500, bbox_inches='tight')
    print('The figure is saved to {}.'.format(output_path))
