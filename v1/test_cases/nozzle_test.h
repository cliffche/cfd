#pragma once
#ifndef _NOZZLE_TEST_H_
#define _NOZZLE_TEST_H_
#include "../one_dimensional_solvers/nozzle.h"
#include "test_case.h"
class NozzleTest :public TestCase
{
public:
	NozzleTest();
	void test() override;
private:

};



#endif // !_NOZZLE_TEST_H_
