#pragma once
#ifndef _COUETTE_TEST_H_
#define _COUETTE_TEST_H_
#include "../one_dimensional_solvers/couette.h"
#include "test_case.h"
class CouetteTest :public TestCase
{
public:
	CouetteTest();
	void test() override;
private:

};



#endif // !_COUETTE_TEST_H_
