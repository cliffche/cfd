#include "jacobi.h"
#include <armadillo>
#include <math.h>
using namespace arma;
using namespace std;

//基于线性代数库 armadillo 的jacobi法数值解线性方程组

/*下一步优化:
1.判断残差自动退出和最大步数不收敛退出√
2.如何高效率(时间,空间)
3.封装*/

//迭代部分
void my_iterator(mat* k, vec* c,double  res) {
	int columns = k->n_cols;//列数
	vec init = zeros(columns);//创建一个全为0的向量
	vec res_v;
	vec rel = zeros(columns);
	bool convergent = false;
	int i = 1;
	while (!convergent || i>100) {
		init = rel;
		rel = *k * init + *c;
		res_v = rel - init;
		cout << "第 " << i << "次迭代结果:" << endl;
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
		cout << "计算至100步仍然不收敛" << endl;
	}
	//return *k;
}
//公式部分
void solver(mat* A,vec* b,double res) {	
	mat U = trimatu(*A, 1);//upper without diagonal
	mat L = trimatl(*A, -1);//lower without diagonal
	mat D = *A - U - L;//diagonal
	mat k = D.i() * (-L - U);//逆矩阵	
	vec c = D.i() * *b;
	my_iterator(&k, &c,res);
}
//测试参数入口
void init() {
	//double类型的matrix
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

