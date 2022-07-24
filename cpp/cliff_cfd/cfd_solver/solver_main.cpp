#include <iostream>
#include <armadillo>
#include <typeinfo>
#include "jacobi.h"
#include "tridiagonal_matrix_algorthim.h"
#include  <time.h>
#include "incompre_couette_one_dimensional.h"

using namespace std;
using namespace arma;


int main11() {
	clock_t start, finish;
	start = clock();
	icod_init();
	finish = clock();
	cout << "运行时间" << (double)(finish - start) / CLOCKS_PER_SEC << endl;
	return 0;
}