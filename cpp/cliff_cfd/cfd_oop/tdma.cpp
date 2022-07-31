#include "tdma.h"
#include <iosfwd>
Tdma::Tdma(const arma::mat& mat, arma::vec& vec) : mat_A(mat), vec_b(vec), vec_cache(vec) {
	this->row = mat.n_rows;
}

arma::vec& Tdma::solve() const {
	//0.处理首行
	mat_A.at(0, 0) = 0;
	mat_A.at(0, 2) /= mat_A.at(0, 1);
	vec_b.at(0) /= mat_A.at(0, 1);
	mat_A.at(0, 1) = 1;
	//1.处理其他行
	for (size_t i = 1; i < row; i++) {
		vec_cache = { mat_A.at(i,0),mat_A.at(i,1), mat_A.at(i,2), vec_b.at(i) };//cache a1 b1 c1 d1		
		mat_A.at(i, 0) = 0;//a2` = 0
		double b_temp = (mat_A.at(i - 1, 1)) * vec_cache.at(1) - vec_cache.at(0) * (mat_A.at(i - 1, 2));
		mat_A.at(i, 1) = 1;
		mat_A.at(i, 2) = vec_cache.at(2) * (mat_A.at(i - 1, 1)) / b_temp;
		vec_b.at(i) = (mat_A.at(i - 1, 1) * vec_cache.at(3) - vec_b.at(i - 1) * vec_cache.at(0)) / b_temp;
	}
	//2.逆序消元一次
	for (int j = row - 2; j >= 0; j--) {
		vec_b.at(j) -= vec_b.at(j + 1) * mat_A.at(j, 2);
		mat_A.at(j, 2) = 0;
	}
	return vec_b;
}
