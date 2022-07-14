#include "jacobi.h"
#include <armadillo>
using namespace arma;
using namespace std;

/*��һ���Ż�:
1.�жϲв��Զ��˳���������������˳�
2.��θ�Ч��(ʱ��,�ռ�)
3.��װ*/
mat my_iterator(mat k, vec c, vec init, vec res) {
	/*mat::iterator it_begin = k.begin_col(0);
	mat::iterator it_end = k.end_col(2);*/
	cout << "k" << k << endl;
	cout << "c" << c << endl;
	vec rel = { 0,0,0 };
	//cout << *it_begin << endl;
	for (int i = 0; i < 6; i++) {
		init = rel;
		rel = k * init + c;
		res = rel - init;
		cout << "i " << i << endl;
		cout << "rel " << rel << endl;
		cout << "res" << res << endl;
	}
	return k;
}

//�������Դ����� armadillo ��jacobi����ֵ�����Է�����
void solver() {
	vec b = { 14,-5,14 };
	//double���͵�matrix
	dmat A = {
		 { 10,3,1 },
		 { 2,-10,3 },
		 { 1,3,10 }
	};
	mat U = trimatu(A, 1);//upper without diagonal
	mat L = trimatl(A, -1);//lower without diagonal
	mat D = A - U - L;//diagonal
	mat k = D.i() * (-L - U);//�����	
	//cout << "A" << A << endl;
	//cout << "L" << L << endl;
	//cout << "D" << D << endl;
	//cout << "u" << U << endl;
	vec c = D.i() * b;
	vec init = { 0,0,0 };
	vec res = { 0.01,0.01,0.01 };
	mat d = my_iterator(k, c, init, res);
}

