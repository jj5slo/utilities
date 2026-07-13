#include <iostream>
#include <cmath>
#include <functional>
#include <algorithm>
/* 注意！　フルAI製で、チェック未了。必ず検算すること。自動アルゴリズムには組み込まない。 */


// 改良版: ブレント法のアルゴリズム実装
double brent_root(std::function<double(double)> func, double a, double b, double x_tol = 1e-7, double y_tol = 1e-7, int max_iter = 1000) {
	double fa = func(a);
	double fb = func(b);

	// 問題点④の修正: 初期値がすでに解である場合
	if (std::abs(fa) < y_tol) return a;
	if (std::abs(fb) < y_tol) return b;

	if (fa * fb > 0.0) {
		std::cerr << "エラー: 指定された区間 [a, b] で符号が反転していません。" << std::endl;
		return NAN;
	}

	// 常に fb が 0 に近くなるようにする（ブレント法の絶対ルール）
	if (std::abs(fa) < std::abs(fb)) {
		std::swap(a, b);
		std::swap(fa, fb);
	}

	double c = a;
	double fc = fa;
	double s = 0;
	double fs = 0;
	bool mflag = true;
	double d = 0;

	for (int iter = 0; iter < max_iter; ++iter) {
		// 問題点②の修正: 区間幅(x)と関数値(y)の両方で収束判定
		if (std::abs(b - a) < x_tol || std::abs(fb) < y_tol) {
			return b;
		}

		// 逆二次補間 または 割線法
		if (fa != fc && fb != fc) {
			s = a * fb * fc / ((fa - fb) * (fa - fc))
			  + b * fa * fc / ((fb - fa) * (fb - fc))
			  + c * fa * fb / ((fc - fa) * (fc - fb));
		} else {
			s = b - fb * (b - a) / (fb - fa);
		}

		// 問題点①の修正: a と b の大小関係に依存しない内分判定
		// s が (3a+b)/4 と b の間に「ない」場合に condition1 を true とする
		double bound1 = (3.0 * a + b) / 4.0;
		double bound2 = b;
		bool condition1 = ((s - bound1) * (s - bound2) >= 0.0);

		bool condition2 = (mflag && std::abs(s - b) >= std::abs(b - c) / 2.0);
		bool condition3 = (!mflag && std::abs(s - b) >= std::abs(c - d) / 2.0);
		bool condition4 = (mflag && std::abs(b - c) < x_tol);
		bool condition5 = (!mflag && std::abs(c - d) < x_tol);

		// 二分法へのフォールバック
		if (condition1 || condition2 || condition3 || condition4 || condition5) {
			s = (a + b) / 2.0;
			mflag = true;
		} else {
			mflag = false;
		}

		fs = func(s);
		d = c;
		c = b;
		fc = fb;

		// 次の区間を設定
		if (fa * fs < 0) {
			b = s;
			fb = fs;
		} else {
			a = s;
			fa = fs;
		}

		// 常に fb が 0 に近い（最良の推測値）状態を保つためのスワップ
		if (std::abs(fa) < std::abs(fb)) {
			std::swap(a, b);
			std::swap(fa, fb);
		}
	}
	
	std::cerr << "警告: 最大反復回数に到達しました。" << std::endl;
	return b;
}
