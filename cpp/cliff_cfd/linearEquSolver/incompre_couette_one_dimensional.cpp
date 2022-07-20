//一维不可压缩couette流 incompre_couette_one_dimensional
#include <iostream>
#include "incompre_couette_one_dimensional.h"
#include <armadillo>
#include <list>
using namespace arma;
using namespace std;
//三对角矩阵的托马斯算法
dmat* solver_tdma_icod(dmat* a) {
	int len = a->n_rows;//行数
	vec vec_cache(len);//缓存项
	//1.顺序消元一次
	//处理首行
	a->at(0, 0) = 0;
	a->at(0, 2) = a->at(0, 2) / a->at(0, 1);
	a->at(0, 3) = a->at(0, 3) / a->at(0, 1);
	a->at(0, 1) = 1;
	//处理其他行
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
	//cout << a->at(18, 3) << endl;
	return a;
}

void icod_solver(dmat* m_mat) {
	//cout << *m_mat << endl;
	dmat* b = solver_tdma_icod(*&m_mat);
	cout << *b << endl;
}

//定义无量纲的计算条件和初始值
void icod_init() {
	const int delta_y = 19;//y方向网格(边界不在里面)
	//构造参数E e = delta_t/(Re * (delta_y **2))
	const int construct_parameter_E = 1;
	//矩阵的参数A
	double param_a = -0.5 * construct_parameter_E;//
	double param_b = 1 + construct_parameter_E;
	double param_c = param_a;
	dmat mat(delta_y, 4);//以 n*4 的形式存储三对角矩阵的A和b
	//初始化条件,最后一点为1,其余为0
	for (int i = 0; i < 60; i++) {
		cout << i << endl;
		if (i == 0) {//第一次迭代,要给d列赋初始值
			//通过链表给矩阵赋值
			list<double> params_list = { param_a, param_b, param_c,0 };
			list<double>::iterator  param_it = params_list.begin();
			dmat::row_iterator row_it = mat.begin_row(0);
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
			mat.at(delta_y - 1, 3) = 0.5 - (-0.5);
		}
		else {//其他次迭代,d列根据前一次计算的结果给出
			double* temp_last = new double(0);
			double* temp_this = new double(0);
			for (int i = 0; i < delta_y; i++) {
				mat.at(i, 0) = param_a;
				mat.at(i, 1) = param_b;
				mat.at(i, 2) = param_c;
				*temp_this = mat.at(i, 3);//缓存上一点的值
				if (i != 0 && i != (delta_y - 1)) {
					mat.at(i, 3) = (1 - construct_parameter_E) * *temp_this + 0.5 * construct_parameter_E * (mat.at(i + 1, 3) + *temp_last);
					*temp_last = *temp_this;
				}
			}
			//处理首尾
			mat.at(delta_y - 1, 3) = (1 - construct_parameter_E) * mat.at(delta_y - 1, 3) + 0.5 * construct_parameter_E * (*temp_last + 1) + 0.5;
			mat.at(0, 3) = (1 - construct_parameter_E) * mat.at(0, 3) + 0.5 * construct_parameter_E * (mat.at(1, 3));
			mat.at(0, 0) = 0;
			mat.at(delta_y - 1, 2) = 0;
		}
		cout << "mat last" << mat.at(delta_y - 1, 3) << endl;
		cout << "mat before cal" << endl;
		cout << mat << endl;
		icod_solver(&mat);
	}
}

/*
优化项目:
1.数值精度的控制
2.内存和速度的优化
3.封装


心得:
1.排除问题首先确认哪一块是经过案例测试没有问题的,例如本算例的线性方程组的解法是无问题的,因此只需要排查边界条件
2.特别要注意边界条件的处理

*/