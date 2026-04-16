#include <iostream>
#include <cmath>      // 数学関数用 (std::sin, std::exp など)
#include <functional> // std::function用

/**
 * 台形公式による数値積分関数
 * * @param func  積分したい関数 f(x)
 * @param a     積分の開始点 (下端)
 * @param b     積分の終了点 (上端)
 * @param n     分割数（大きいほど精度が上がるが計算量が増える）
 * @return      積分の近似値
 */
double integrate(const std::function<double(double)>& func, double a, double b, int n) {
    if (n <= 0) return 0.0;

    double h = (b - a) / n;         // 各区間の幅 (刻み幅)
    double sum = 0.5 * (func(a) + func(b)); // 両端の点の計算 (1/2倍)

    // 内側の点の計算
    for (int i = 1; i < n; ++i) {
        double x = a + h * i;
        sum += func(x);
    }

    return sum * h;
}

int main() {
    // 例1: f(x) = x^2 の積分 (0から1まで)
    // 解析解は 1/3 (約 0.3333...)
    auto square = [](double x) { return x * x; };
    double result1 = integrate(square, 0.0, 1.0, 1000);
    
    std::cout << "f(x) = x^2 (0 -> 1): " << result1 << std::endl;


    // 例2: f(x) = sin(x) の積分 (0からPIまで)
    // 解析解は 2.0
    double pi = std::acos(-1.0);
    double result2 = integrate(std::sin, 0.0, pi, 1000);

    std::cout << "f(x) = sin(x) (0 -> PI): " << result2 << std::endl;

    return 0;
}
