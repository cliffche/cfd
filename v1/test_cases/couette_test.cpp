#include <iostream>
#include "couette_test.h"

CouetteTest::CouetteTest() {
}

void CouetteTest::test() {
	Couette* couette_solver = new Couette(19, 240, 0.2, 64);
	couette_solver->solve();
}