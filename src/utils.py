import os


def load_params():
    file_path = './config.h'
    with open(file_path, mode='r') as f:
        lines = f.readlines()
        lines = [line.strip() for line in lines if line.strip()]
    params = {
        'POP_SIZE': int(lines[2].split()[2]),
        'EPOCH': int(lines[3].split()[2]),
        'DEPTH': int(lines[4].split()[2]),
        'WIDTH': int(lines[5].split()[2]),
        'TIMES': int(lines[6].split()[2]),
        'N': int(lines[7].split()[2]),
        'M': int(lines[8].split()[2]),
    }
    params['K'] = [int(lines[9 + i].split()[2]) for i in range(params['M'])]
    return params


def make_dir(path):
    if not os.path.isdir(path):
        os.makedirs(path)


if __name__ == '__main__':
    params = load_params()
    print(params)
    print(''.join([str(k) for k in params['K']]))
