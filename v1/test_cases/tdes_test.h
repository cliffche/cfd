#pragma once
#ifndef _TDES_TEST_H_
#define _TDES_TEST_H_
#include "../two_dimensional_solvers/two_dimensional_explicit_solver.h"
#include "test_case.h"
class TdesTest :public TestCase
{
public:
	TdesTest();
	void testCase() override;
private:

};



#endif // !_TDES_TEST_H_
