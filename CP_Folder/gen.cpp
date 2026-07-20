#include<bits/stdc++.h>
using namespace std;
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt")
uint64_t seed = chrono::steady_clock::now().time_since_epoch().count();
inline uint64_t rng() {
    uint64_t z = (seed += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    return (z ^ (z >> 27)) * 0x94d049bb133111eb;
}
inline long long rnd(long long l, long long r) {
    return l + rng() % (r - l + 1);
}
int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);
    int n = rnd(1, 6000); 
    cout << n << "\n";
    for (int i = 0; i < n; ++i) {
        cout << rnd(-1000, 1000) << " ";
    }
    return 0;
}
