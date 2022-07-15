#include "jacobi.h"
#include <armadillo>
#include <math.h>
using namespace arma;
using namespace std;

//�������Դ����� armadillo ��jacobi����ֵ�����Է�����

/*��һ���Ż�:
1.�жϲв��Զ��˳���������������˳���
2.��θ�Ч��(ʱ��,�ռ�)
3.��װ*/

//��������
void my_iterator(mat* k, vec* c,double  res) {
	int columns = k->n_cols;//����
	vec init = zeros(columns);//����һ��ȫΪ0������
	vec res_v;
	vec rel = zeros(columns);
	bool convergent = false;
	int i = 1;
	while (!convergent || i>100) {
		init = rel;
		rel = *k * init + *c;
		res_v = rel - init;
		cout << "�� " << i << "�ε������:" << endl;
		cout << "rel " << rel << endl;
		cout << "res" << res_v << endl;
		vec::iterator it_v = res_v.begin();
		vec::iterator it_v_end = res_v.end();
		while (it_v != it_v_end) {
			if (abs( * it_v) >  res) {
				convergent = false;
				break;
			}
			it_v++;
			convergent = true;
		}
		i++;
	}
	if (!convergent) {
		cout << "������100����Ȼ������" << endl;
	}
	//return *k;
}
//��ʽ����
void solver(mat* A,vec* b,double res) {	
	mat U = trimatu(*A, 1);//upper without diagonal
	mat L = trimatl(*A, -1);//lower without diagonal
	mat D = *A - U - L;//diagonal
	mat k = D.i() * (-L - U);//�����	
	vec c = D.i() * *b;
	my_iterator(&k, &c,res);
}
//���Բ������
void init() {
	//double���͵�matrix
	dmat A ={ 
		{ 10,3,1 ,5},
		{ 2,-10,3,4 },
		{ 1,3,10,3 },
		{ 2,1,5,4 },
	};
	vec b = { 14,-5,14,3 };
	double res = 0.001;
	solver(&A,&b,res);
}

