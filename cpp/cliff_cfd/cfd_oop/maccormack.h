#pragma once
#ifndef _MACCORMACK_H_
#define _MACCORMACK_H_
#include <map>
#include <vector>
#include <string>
#include <ostream>
using map_point = std::map<std::string, double>; //use map structure to develop the program for code semanticsand development efficiency
//maccormack推进方法,二维完整n-s方程
class Maccormack
{
public:
	Maccormack(size_t, size_t);
	friend std::ostream& operator<<(std::ostream&, Maccormack&);
	void initial();//initialization condition
	void boundary();//boundary conditions

private:
	std::vector<std::vector<map_point>> points;
	void timestepCalculator();//calTimeStep
	void maccormackPush();//cal n+1 value by maccormack method
	void convergenceJudgement();//judge whether the residuals convergence
	void tau_xx_Calculator();//calculate tau_xx 
	void tau_xy_Calculator();//calculate tau_xx 
	void tau_yy_Calculator();//calculate tau_xx 

};
std::ostream& operator<<(std::ostream&, Maccormack&);
#endif // !_MACCORMACK_H_

