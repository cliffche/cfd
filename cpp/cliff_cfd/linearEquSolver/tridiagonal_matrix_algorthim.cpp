//不带扰动的三对角矩阵解法 Thmoas法
#include <iostream>
#include <armadillo>
#include "tridiagonal_matrix_algorthim.h"
using namespace std;
using namespace arma;


void solver_tdma(mat* a) {
	cout << *a << endl;
	vec cache = { 0,0,0,0 };//缓存项(a2,b2,c2)
	int len = a->n_rows;
	//顺序消元依次
	for (int i = 1; i < len; i++) {				
		cache = { a->at(i,0),a->at(i,1), a->at(i,2), a->at(i,3) };//缓存
		cout << *a << endl;
		a->at(i, 0) = 0;
		a->at(i, 1) = (a->at(i - 1, 1)) * cache.at(1)- cache.at(0)* (a->at(i - 1, 2));
		a->at(i, 2) = cache.at(2) * (a->at(i - 1, 1));
		a->at(i, 3) = a->at(i - 1, 1) * cache.at(3) - a->at(i - 1, 3) * cache.at(0);
	}
	//最后两列消掉一个元素
	
	//逆序消元一次
	
	//求解

	cout << *a << endl;

}

void init_tdma() {
	dmat a = {//简化存储三对角矩阵,对于大型三对角矩阵可以节省内存{a1,b1,c1,d1}
		{1.1 ,1.2 ,0.0 ,1},
		{1.2 ,1.3 ,1.4 ,2},
		{2.3 ,4.1 ,2.5 ,3},
		{1.7 ,1.4 ,1.5 ,4},
		{0.0 ,2.4 ,3.5 ,5}
	};
	solver_tdma(&a);
}

/*	cout <<"cache"<< cache << endl;
	cout <<"a1: "<< a->at(i - 1, 0) << endl;
	cout <<"b1: "<< a->at(i - 1, 1) << endl;
	cout <<"c1: "<< a->at(i - 1, 2) << endl;
	cout <<"d1: "<< a->at(i - 1, 3) << endl;
	cout << "a2: " << cache.at(0) << endl;
	cout << "b2: " << cache.at(1) << endl;
	cout << "c2: " << cache.at(2) << endl;
	cout << "d2: " << cache.at(3) << endl;
	*/
