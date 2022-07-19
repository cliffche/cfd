//一维不可压缩couette流 incompre_couette_one_dimensional
#include <iostream>
#include "incompre_couette_one_dimensional.h"
#include <armadillo>
#include <list>
using namespace arma;
using namespace std;
mat* solver_tdma_icod(mat* a) {
	cout << *a << endl;

	int len = a->n_rows;//行数
	vec vec_cache(len);//缓存项
	//1.顺序消元一次
	for (int i = 1; i < len; i++) {
		vec_cache = { a->at(i,0),a->at(i,1), a->at(i,2), a->at(i,3) };//缓存a1 b1 c1 d1
		//cout << *a << endl;
		a->at(i, 0) = 0;//a2` = 0
		//b2` = b1*b2 - a2*c1 实际迭代发现b2`必须化成1,因为过大的矩阵会导致后面的项存储溢出 a->at(i, 1) = (a->at(i - 1, 1)) * vec_cache.at(1) - vec_cache.at(0) * (a->at(i - 1, 2));
		double b_temp = (a->at(i - 1, 1)) * vec_cache.at(1) - vec_cache.at(0) * (a->at(i - 1, 2));
		a->at(i, 1) = 1;
		a->at(i, 2) = vec_cache.at(2) * (a->at(i - 1, 1)) / b_temp;//c2` = b1 * c2 /(b1*b2 - a2*c1)
		a->at(i, 3) = (a->at(i - 1, 1) * vec_cache.at(3) - a->at(i - 1, 3) * vec_cache.at(0)) / b_temp; // d2` = (b1*d2 - a2*d1) /(b1*b2 - a2*c1)
	}
	//2.逆序消元一次
	for (int j = len - 2; j >= 0; j--) {
		//tdma_soluction.at(j) = ;//d3`` = d3` - d4``*c3
		a->at(j, 3) = a->at(j, 3) - a->at(j + 1, 3) * a->at(j, 2);
		a->at(j, 2) = 0;
	}

	//cout << *a << endl;
	return a;
}

void icod_solver(mat* m_mat) {
	//cout << *m_mat << endl;
	mat* b = solver_tdma_icod(*&m_mat);
	cout << "* b" << endl;
	cout << *b << endl;
	mat::row_iterator row_it = b->begin_row(0);
	const int construct_parameter_E = 1;//构造参数E e = delta_t/(Re * (delta_y **2))
	//初始化条件,最后一点为1,其余为0
	double param_a = -0.5 * construct_parameter_E;//
	double param_b = 1 + construct_parameter_E;
	double param_c = param_a;
	int i = -1;
	while (row_it != b->end_row(20))
	{
		i++;
		cout << i << endl;
		switch (i)
		{
		case 0:
			*row_it = param_a;
		case 1:
			*row_it = param_b;
		case 2:
			*row_it = param_c;
		case 3:
			i = -1;
		default:
			break;
		}
		row_it++;
	}
	cout << "* b" << endl;
	cout << *b << endl;

}
//定义无量纲的计算条件和初始值
void icod_init() {
	const int delta_y = 21;//y方向网格
	const int construct_parameter_E = 1;//构造参数E e = delta_t/(Re * (delta_y **2))
	//初始化条件,最后一点为1,其余为0
	vec vec_k(delta_y, fill::zeros);
	vec_k.at(delta_y - 1) = 1;
	double param_a = -0.5 * construct_parameter_E;//
	double param_b = 1 + construct_parameter_E;
	double param_c = param_a;
	mat mat(delta_y, 4);
	//通过链表给矩阵赋值
	list<double> params_list = { param_a, param_b, param_c,0 };
	list<double>::iterator  param_it = params_list.begin();
	mat::row_iterator row_it = mat.begin_row(0);
	while (row_it != mat.end_row(delta_y - 1))
	{
		*row_it = *param_it;
		param_it++;
		if (param_it == params_list.end()) {
			param_it = params_list.begin();
		}
		row_it++;
	}
	//修改第一行和最后一行
	mat.at(0, 0) = 0;
	mat.at(delta_y - 1, 2) = 0;
	mat.at(delta_y - 1, 3) = 1;
	icod_solver(&mat);

}

