#pragma once
#ifndef _MACCORMACK_H_
#define _MACCORMACK_H_
#include <map>
#include <string>
//maccormack推进方法,二维完整n-s方程

class maccormack
{
public:
	maccormack();
	~maccormack();

private:
	std::map<std::string, double> point;

};

maccormack::maccormack()
{


	maccormack::~maccormack()
	{
	}

#endif // !_MACCORMACK_H_

