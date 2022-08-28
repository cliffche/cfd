#include <iostream>
#include "tdes_test.h"

TdesTest::TdesTest() {
}

void TdesTest::test() {
	Tdes* n_s_solver = new Tdes(10, 10);
	n_s_solver->setBoundary(298.0, 298.0, 101325.0, 340.0);
	n_s_solver->solve(*n_s_solver);
	//std::cout << *n_s_solver << std::endl;
}