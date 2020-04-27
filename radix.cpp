#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

int compare(const void * a, const void * b) {
    return (*(int*)a - *(int*)b);
}

vector<int> generate_array(int n) {
    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        v[i] = rand() % int(1e5);
    }
    
    return v;
}

int get_byte(int n, int ind) {
    int x = (n >> (8 * ind)) & 255;

    return x;
}

void count_sort(vector<int> &v, int ind) {
    int cnt[256];
    for (int i = 0;i < 256; ++i)
        cnt[i] = 0;
    int n = v.size();
    for (int i = 0; i < n; ++i) {
        int x = get_byte(v[i], ind);
        ++cnt[get_byte(v[i], ind)];
    }

    for (int i = 1; i < 256; ++i) {
        cnt[i] += cnt[i - 1];
    }

    vector<int> res(n);
    for (int i = n - 1; i >= 0; --i) {
        res[--cnt[get_byte(v[i], ind)]] = v[i];
    }

    v = res;
}

void radix(vector<int> &v) {
    for (int i = 0; i < 4; ++i) {
        count_sort(v, i);
    }
}

int main() {
    freopen("radix_out.txt", "w", stdout);
    srand(time(0));
    int n;
    while (cin >> n) {
        for (int k = 0; k < 7; ++k) {
            vector<int> v = generate_array(n);

            vector<int> s = v, qs = v;

            clock_t tr_st = clock();
            radix(v);
            clock_t tr_ed = clock();

            double res_r = double(tr_ed - tr_st) / CLOCKS_PER_SEC;

            clock_t ts_st = clock();
            sort(s.begin(), s.end());
            clock_t ts_ed = clock();

            double res_s = double(ts_ed - ts_st) / CLOCKS_PER_SEC;

            clock_t tsq_st = clock();
            qsort(&qs[0], n, sizeof(int), compare);
            clock_t tsq_ed = clock();

            double res_sq = double(tsq_ed - tsq_st) / CLOCKS_PER_SEC;

            cout << n << endl;

            cout << "Radix: " << fixed << setprecision(10) << res_r << " --- ";
            cout << "Sort: " << fixed << setprecision(10) << res_s << " --- ";
            cout << "QSort: " << fixed << setprecision(10) << res_sq << endl;
        }
    }
}
// 10 77 243 987 3498 13546 50182 77648 117065 250307 1023569 12402210