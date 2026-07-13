#include <iostream>
#include <cmath>
#include <functional>

#include "brent_root_finding.h"


int main(void){
	{
		double A_alpha = 10.0;
		double A_beta = 5.0;
		double C_alpha = 100.0;
		double C_beta = 10.0;

		double K = std::log(C_alpha) / std::log(C_beta);

		auto f = [A_alpha, A_beta, K](double b) {
			return std::log(A_alpha - b) - K * std::log(A_beta - b);
		};

		double lower_bound = -100.0;
		double upper_bound = std::min(A_alpha, A_beta) - 0.001;

		// xの許容誤差とyの許容誤差をそれぞれ設定
		double solution = brent_root(f, lower_bound, upper_bound, 1e-8, 1e-8);

		if (!std::isnan(solution)) {
			std::cout << "値の比 K: " << K << std::endl;
			std::cout << "解 b: " << solution << std::endl;
			std::cout << "検算 f(b): " << f(solution) << std::endl;
			std::cout << "検算 K: " << std::log(A_alpha - solution) / std::log(A_beta - solution) << std::endl;
		}
	}
	double A_alpha = 25.2;
	double A_beta = 23.7;
	double C_alpha = 0.000576248;
	double C_beta = 0.000295255;

	double K = std::log(C_alpha) / std::log(C_beta);

	auto f = [A_alpha, A_beta, K](double b) {
		return std::log(A_alpha - b) - K * std::log(A_beta - b);
	};

	double lower_bound = -100.0;
	double upper_bound = 100;

	// xの許容誤差とyの許容誤差をそれぞれ設定
	double solution = brent_root(f, lower_bound, upper_bound, 1e-8, 1e-8);

	if (!std::isnan(solution)) {
		std::cout << "値の比 K: " << K << std::endl;
		std::cout << "解 b: " << solution << std::endl;
		std::cout << "検算 f(b): " << f(solution) << std::endl;
		std::cout << "検算 K: " << std::log(A_alpha - solution) / std::log(A_beta - solution) << std::endl;
	}
	return 0;
}
	
