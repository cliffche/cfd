//�����Ŷ������ԽǾ���ⷨ Thmoas��
#include <iostream>
#include <armadillo>
#include "tridiagonal_matrix_algorthim.h"
using namespace std;
using namespace arma;


void solver_tdma(mat* a) {
	cout << *a << endl;

	int len = a->n_rows;//����
	vec vec_cache(len);//������
	//1.˳����Ԫһ��
	for (int i = 1; i < len; i++) {
		vec_cache = { a->at(i,0),a->at(i,1), a->at(i,2), a->at(i,3) };//����a1 b1 c1 d1
		//cout << *a << endl;
		a->at(i, 0) = 0;//a2` = 0
		//b2` = b1*b2 - a2*c1 ʵ�ʵ�������b2`���뻯��1,��Ϊ����ľ���ᵼ�º������洢��� a->at(i, 1) = (a->at(i - 1, 1)) * vec_cache.at(1) - vec_cache.at(0) * (a->at(i - 1, 2));
		double b_temp = (a->at(i - 1, 1)) * vec_cache.at(1) - vec_cache.at(0) * (a->at(i - 1, 2));
		a->at(i, 1) = 1;
		a->at(i, 2) = vec_cache.at(2) * (a->at(i - 1, 1)) / b_temp;//c2` = b1 * c2 /(b1*b2 - a2*c1)
		a->at(i, 3) = (a->at(i - 1, 1) * vec_cache.at(3) - a->at(i - 1, 3) * vec_cache.at(0)) / b_temp; // d2` = (b1*d2 - a2*d1) /(b1*b2 - a2*c1)
	}
	//2.������Ԫһ��
	for (int j = len - 2; j >= 0; j--) {
		//tdma_soluction.at(j) = ;//d3`` = d3` - d4``*c3
		a->at(j, 3) = a->at(j, 3) - a->at(j + 1, 3) * a->at(j, 2);
		a->at(j, 2) = 0;
	}
	cout << *a << endl;
}

void init_tdma() {
	//ѹ���洢���ԽǾ����Ӧ�����Է�����Ax = b,���ڴ������ԽǾ�����Խ�ʡ�ڴ�{a,b,c,d}
	dmat a = {
		{0,1,2,3},
		{2,3,4,9},
		{3,4,5,12},
		{4,5,6,15},
		{6,7,0,13}
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
