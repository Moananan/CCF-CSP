#include <bits/stdc++.h>
using namespace std;

// 把 "12.345" 这样的十进制字符串解析成整数 (乘 1000 后)
static long long parseMilli(const string &s) {
    size_t p = s.find('.');
    string ip = (p == string::npos) ? s : s.substr(0, p);
    string fp = (p == string::npos) ? "" : s.substr(p + 1);
    while (fp.size() < 3) fp.push_back('0');
    long long v = 0;
    for (char c : ip) if (isdigit((unsigned char)c)) v = v * 10 + (c - '0');
    long long f = 0;
    for (int i = 0; i < 3; i++) f = f * 10 + (fp[i] - '0');
    return v * 1000 + f;
}

// 把 "1.5" 这样的字符串解析成整数 (乘 10 后)
static int parseDeci(const string &s) {
    size_t p = s.find('.');
    string ip = (p == string::npos) ? s : s.substr(0, p); //整数部分
    string fp = (p == string::npos) ? "" : s.substr(p + 1); //小数部分
    while (fp.empty()) fp.push_back('0');
    int v = 0;
    for (char c : ip) if (isdigit((unsigned char)c)) v = v * 10 + (c - '0');
    return v * 10 + (fp[0] - '0');
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    string qs;
    if (!(cin >> n >> m >> qs)) return 0;
    // n: 物品数量, m: 限制条件, qs: 约束参数

    vector<int> mu(n);
    for (int i = 0; i < n; i++) cin >> mu[i]; // 每件 "收益" = mu_i

    vector<int> w(n);           // 每件 "重量" = 100 * sigma_i^2 = k^2
    int totW = 0;
    for (int i = 0; i < n; i++) {
        string ss;
        cin >> ss;
        int k = parseDeci(ss);  // k = 10 * sigma_i, 范围 1..30
        w[i] = k * k;
        totW += w[i];
    }

    // 约束 sum(k_i^2) <= 100 * m^2 / q^2, q = Q/1000
    // 即 sum(k_i^2) <= 10^8 * m^2 / Q^2  (全部为正整数, 整除就是下取整)
    long long Q = parseMilli(qs);
    long long cap = (long long)(100000000LL * m * m / (Q * Q));
    if (cap > totW) cap = (int)totW;   // 容量不需要超过总重量

    // 0/1 背包: dp[j] = 总重量恰为 j 时的最大收益
    const long long NEG = -(long long)4e18; //哨兵值，表示DP状态不可达
    vector<long long> dp(cap + 1, NEG);
    dp[0] = 0;
    for (int i = 0; i < n; i++) {
        long long wi = w[i], vi = mu[i];
        for (int j = cap; j >= wi; j--)
            if (dp[j - wi] != NEG)
                dp[j] = max(dp[j], dp[j - wi] + vi);
    }

    long long ans = 0;                 // 空集: 收益 0
    for (int j = 0; j <= cap; j++) ans = max(ans, dp[j]);
    cout << ans << '\n';
    return 0;
}
