#include <iostream>
#include <functional>
#include <cmath>
using namespace std;

double f1(double x) {
    if (x == -1) {
        x = 0;
    }
    return 3 * (0.5 / (x + 1) + 1);
}

double f2(double x) {
    return 2.5 * x - 9.5;
}

double f3(double x) {
    return 5 / x;
}

double f1_minus_f2(double x) {
    return 3 * (0.5 / (x + 1) + 1) - 2.5 * x + 9.5;
}

double f1_minus_f2_d1(double x) {
    return -1.5 / ((x + 1) * (x + 1)) - 2.5;
}

double f1_minus_f2_d2(double x) {
    return 3 / ((x + 1) * (x + 1) * (x + 1));
}

double f1_minus_f3(double x) {
    return 3 * (0.5 / (x + 1) + 1) - 5 / x;
}

double f1_minus_f3_d1(double x) {
    return 3 * (0.5 / (x + 1) + 1) - (-5) / (x * x);
}

double f1_minus_f3_d2(double x) {
    return 3 / ((x + 1) * (x + 1) * (x + 1)) - 10 / (x * x * x);
}

double f2_minus_f3(double x) {
    return 2.5 * x - 9.5 - 5 / x;
}

double f2_minus_f3_d1(double x) {
    return 2.5 - (-5) / (x * x);
}

double f2_minus_f3_d2(double x) {
    return - 10 / (x * x * x);
}

double roots_newton(function<double(double)> f, function<double(double)> fd, double a, double eps = 0.0001) {
    double x1  = a - f(a) / fd(a);
    double x0 = a;
    while (fabs(x0 - x1) > eps) {
        x0 = x1;
        x1 = x1 - f(x1) / fd(x1);
    }
    return x1;
}

double rndDouble(double a, double b) {
    return a + double(rand()) / RAND_MAX * (b - a);
}

void fillMaxMinValues(function<double (double)> f, double a, double b, double &mn, double &mx) {
    mx = -1e18;
    mn = 1e18;
    for (double X = a; X <= b; X += 0.0001) {
        double valX = f(X);
        mx = std::max(mx, valX);
        mn = std::min(mn, valX);
    }
}

double integral_monte(function<double(double)> f, double x_min, double x_max, double iterations = 1e8) {
    double y_min, y_max, cnt = 0, m = 0;
    fillMaxMinValues(f, x_min, x_max, y_min, y_max);
    bool sign = 1;
    if (y_max <= 0) {
        y_max = fabs(y_min);
        sign = 0;
    }
    y_min = 0;
    for (cnt = 0; cnt < iterations; cnt++) {
        double x = rndDouble(x_min, x_max), y = rndDouble(y_min, y_max), val = fabs(f(x));
        if (val > y)
            ++m;
    }
    double res = (x_max - x_min) * (y_max - y_min) * (m / iterations);
    if (!sign) {
        return -res;
    }
    return res;
}

double integral_riemann_sum(function<double(double)> f, double x_min, double x_max, double n = 1e7) {
    double h = (x_max - x_min) / n;
    double S = 0;
    
    for (int i = 0; i < n; ++i) {
        S += f(x_min + h * (i + 0.5));
    }

    return S * h;
}

double riemann_sum_solve() {
    double f1_2 = roots_newton(f1_minus_f2, f1_minus_f2_d1, 3);
    double f1_3 = roots_newton(f1_minus_f3, f1_minus_f3_d1, 1);
    double f2_3 = roots_newton(f2_minus_f3, f2_minus_f3_d1, 1);
    double S1 = integral_riemann_sum(f1, f1_3, f1_2);
    double S2 = integral_riemann_sum(f3, f1_3, f2_3);
    double S3 = integral_riemann_sum(f2, f2_3, f1_2);
    return S1 - S2 - S3;
}

double solve_monte_carlo() {
    double f1_2 = roots_newton(f1_minus_f2, f1_minus_f2_d1, 1);
    double f1_3 = roots_newton(f1_minus_f3, f1_minus_f3_d1, 1);
    double f2_3 = roots_newton(f2_minus_f3, f2_minus_f3_d1, 1);
    double S1 = integral_monte(f1, f1_3, f1_2);
    double S2 = integral_monte(f3, f1_3, f2_3);
    double S3 = integral_monte(f2, f2_3, f1_2);
    return S1 - S2 - S3;
}

int main() {
    srand(unsigned(time(0)));
    cout << "f1 & f2: " << roots_newton(f1_minus_f2, f1_minus_f2_d1, 1) << endl;
    cout << "f1 & f3: " << roots_newton(f1_minus_f3, f1_minus_f3_d1, 1) << endl;
    cout << "f2 & f3: " << roots_newton(f2_minus_f3, f2_minus_f3_d1, 1) << endl;
    
    double riemann_sum_method = riemann_sum_solve();
    cout << "S by Riemann Sum Method: " << riemann_sum_method << endl;
    
    double s_monte = solve_monte_carlo();
    cout << "S by Monte-Carlo: " << s_monte << endl;
    cout << "Diff: " << fabs(riemann_sum_method - s_monte) << endl;
}
