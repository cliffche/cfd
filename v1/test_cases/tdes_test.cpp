#include <iostream>
#include "tdes_test.h"

TdesTest::TdesTest() {
}

void TdesTest::test() {
	Tdes* n_s_solver = new Tdes(70, 70);
	n_s_solver->setBoundary(298.0, 298.0, 101325.0, 680.0);
	n_s_solver->setInitial();
	n_s_solver->solve();
	std::cout << &n_s_solver << std::endl;
}