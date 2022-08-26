#include <iostream>
#include "nozzle_test.h"

NozzleTest::NozzleTest() {
}

void NozzleTest::test() {
	Nozzle* nozzle_solver = new Nozzle(31, 1500, 3, 2.2, -6.6, 5.95, 1.4, 0.5);// ��������, ��ʱ�䲽, x_len, A(x)2����,1����,������,gama,Courant��
	nozzle_solver->init_calc_parameter(-0.3146, 1, -0.2314, 1, 1.09, 0.1);//��ʼ����
	nozzle_solver->solve();
	nozzle_solver->cout_result();
}