#include <iostream>
#include <typeinfo>
#include "jacobi.h"
#include "tridiagonal_matrix_algorthim.h"
#include  <time.h>
#include "incompre_couette_one_dimensional.h"
#include "quasi_one_dimensional_nozzle_flow.h"
using namespace std;


int main() {
	clock_t start, finish;
	start = clock();
	//icod_init();//couette流
	//init_tdma();//三对角矩阵
	nozzle_init();//喷管问题
	finish = clock();
	cout << "运行时间" << (double)(finish - start) / CLOCKS_PER_SEC << endl;
	return 0;
}