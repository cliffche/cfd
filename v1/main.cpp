#include <iostream>
#include "test_cases/tdes_test.h"
#include "test_cases/couette_test.h"
#include "test_cases/nozzle_test.h"
int main() {

	CouetteTest* couette_test = new CouetteTest();
	couette_test->test();

	NozzleTest* nozzle_test = new NozzleTest();
	nozzle_test->test();

	TestCase* tdes_test = new TdesTest();
	tdes_test->test();

	return 0;
}