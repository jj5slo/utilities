
/* 注意！　フルAI製で、チェック未了。必ず検算すること。自動アルゴリズムには組み込まない。 */
double brent_root(std::function<double(double)> func, double a, double b, double x_tol = 1e-7, double y_tol = 1e-7, int max_iter = 1000);
