#pragma once
#ifndef _TEST_CASE_H_
#define _TEST_CASE_H_
class TestCase
{
public:
	TestCase() = default;
	virtual ~TestCase();
	void virtual test() = 0;
private:

};


#endif // !_TEST_CASE_H_
