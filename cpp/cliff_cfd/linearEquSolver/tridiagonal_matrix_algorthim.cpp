//不带扰动的三对角矩阵解法 Thmoas法
#include <iostream>
#include <armadillo>
#include "tridiagonal_matrix_algorthim.h"
using namespace std;
using namespace arma;


void solver_tdma(mat* a) {
	cout << *a << endl;
	vec vec_cache = { 0,0,0,0 };//缓存项
	int len = a->n_rows;
	//1.顺序消元依次
	for (int i = 1; i < len; i++) {				
		vec_cache = { a->at(i,0),a->at(i,1), a->at(i,2), a->at(i,3) };//缓存
		cout << *a << endl;
		a->at(i, 0) = 0;
		a->at(i, 1) = (a->at(i - 1, 1)) * vec_cache.at(1)- vec_cache.at(0)* (a->at(i - 1, 2));
		a->at(i, 2) = vec_cache.at(2) * (a->at(i - 1, 1));
		a->at(i, 3) = a->at(i - 1, 1) * vec_cache.at(3) - a->at(i - 1, 3) * vec_cache.at(0);
	}
	//2.最后两列消掉一个元素
	
	//逆序消元一次
	
	//求解

	cout << *a << endl;

}

void init_tdma() {
	dmat a = {//简化存储三对角矩阵对应的线性方程组Ax = b,对于大型三对角矩阵可以节省内存{a,b,c,d}
		{1.1 ,1.2 ,0.0 ,1},
		{1.2 ,1.3 ,1.4 ,2},
		{2.3 ,4.1 ,2.5 ,3},
		{1.7 ,1.4 ,1.5 ,4},
		{0.0 ,2.4 ,3.5 ,5}
	};
	solver_tdma(&a);
}

/*	cout <<"vec_cache"<< vec_cache << endl;
	cout <<"a1: "<< a->at(i - 1, 0) << endl;
	cout <<"b1: "<< a->at(i - 1, 1) << endl;
	cout <<"c1: "<< a->at(i - 1, 2) << endl;
	cout <<"d1: "<< a->at(i - 1, 3) << endl;
	cout << "a2: " << vec_cache.at(0) << endl;
	cout << "b2: " << vec_cache.at(1) << endl;
	cout << "c2: " << vec_cache.at(2) << endl;
	cout << "d2: " << vec_cache.at(3) << endl;
	*/
