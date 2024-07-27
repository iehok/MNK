#ifndef UTILS_H_
#define UTILS_H_

#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

template <typename... Args>
string format(const string& fmt, Args... args) {
    size_t len = snprintf(nullptr, 0, fmt.c_str(), args...);
    vector<char> buf(len + 1);
    snprintf(&buf[0], len + 1, fmt.c_str(), args...);
    return string(&buf[0], &buf[0] + len);
}

void current_time() {
    time_t t = time(nullptr);
    const tm* lt = localtime(&t);
    printf("%d/%02d/%02d/%02d:%02d:%02d\n", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
}

vector<int> encode(long long dec, int n) {
    vector<int> res(n);
    for (int i = 0; i < n; i++) {
        res[i] = dec / (1LL << (n - i - 1));
        dec %= 1LL << (n - i - 1);
    }
    return res;
}

long long decode(const vector<int>& a) {
    long long res = 0;
    for (int i = 0; i < a.size(); i++) res += a[i] * (1LL << (a.size() - i - 1));
    return res;
}

int hamming_distance(const vector<int>& a, const vector<int>& b) {
    int dis = 0;
    for (int i = 0; i < a.size(); i++)
        if (a[i] != b[i]) dis++;
    return dis;
}

#endif
