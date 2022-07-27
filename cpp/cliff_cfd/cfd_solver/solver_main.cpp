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
	//icod_init();//couette��
	//init_tdma();//���ԽǾ���
	nozzle_init();//�������
	finish = clock();
	cout << "����ʱ��" << (double)(finish - start) / CLOCKS_PER_SEC << endl;
	return 0;
}