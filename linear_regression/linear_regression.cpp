#include <cmath>

#include "linear_regression.h"

// 回帰直線の係数 a, b を計算して返す関数 (y = ax + b)
// 戻り値: result[0] = a (傾き), result[1] = b (切片)
double* linear_regression(double* x, double* y, int N) {
	// データが2つ未満の場合は直線を引けないため nullptr を返す
	if (N < 2) {
		return nullptr;
	}

	double sum_x = 0.0;
	double sum_y = 0.0;
	double sum_xy = 0.0;
	double sum_xx = 0.0;

	for (int i = 0; i < N; ++i) {
		sum_x += x[i];
		sum_y += y[i];
		sum_xy += x[i] * y[i];
		sum_xx += x[i] * x[i];
	}

	// 分母の計算
	double denominator = N * sum_xx - sum_x * sum_x;

	// ゼロ除算の回避（xが全て同じ値で垂直な直線になる場合）
	if (denominator == 0.0) {
		return nullptr;
	}

	// 係数の計算
	double a = (N * sum_xy - sum_x * sum_y) / denominator;
	double b = (sum_y - a * sum_x) / N;

	// 結果を格納する配列を動的確保して返す
	double* result = new double[2];
	result[0] = a;
	result[1] = b;

	return result;
}

// 回帰直線の係数 a, b を計算して返す関数 (y = ax + b)
// 戻り値: result[0] = a (傾き), result[1] = b (切片)
double* log_regression(double* x, double* y, int N) {
	// データが2つ未満の場合は直線を引けないため nullptr を返す
	if (N < 2) {
		return nullptr;
	}
	for(int i=0; i<N; ++i){
		y[i] = std::log10(y[i]);
	}

	double sum_x = 0.0;
	double sum_y = 0.0;
	double sum_xy = 0.0;
	double sum_xx = 0.0;

	for (int i = 0; i < N; ++i) {
		sum_x += x[i];
		sum_y += y[i];
		sum_xy += x[i] * y[i];
		sum_xx += x[i] * x[i];
	}

	// 分母の計算
	double denominator = N * sum_xx - sum_x * sum_x;

	// ゼロ除算の回避（xが全て同じ値で垂直な直線になる場合）
	if (denominator == 0.0) {
		return nullptr;
	}

	// 係数の計算
	double a = (N * sum_xy - sum_x * sum_y) / denominator;
	double b = (sum_y - a * sum_x) / N;

	// 結果を格納する配列を動的確保して返す
	double* result = new double[2];
	result[0] = a;
	result[1] = b;

	return result;
}
