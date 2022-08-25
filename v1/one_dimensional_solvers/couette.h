#pragma once
#ifndef _COUETTE_H_
#define _COUETTE_H_
#include "../utils/tdma.h"
class Couette
{
public:
	Couette(size_t, size_t, double, double);
	void compute() const;
	void matrix_init();//根据计算条件初始化矩阵A和列向量b
	void update_vecb();//更新Ax=b 的b列向量
	void solve(); //求解步骤控制程序
private:
	const size_t mesh_qty;
	const size_t total_time_step;
	const double delta_t;
	const double Reynold_number;
	mutable double construct_parameter_E;
	double delta_y;
	arma::mat mat_A;
	mutable arma::vec vec_b;
};

#endif // !_COUETTE_H_
