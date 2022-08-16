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
	size_t total_timestep = 1000;
	double courant = 0.7;//courant number
	double gama = 1.4;//gama default air
	double pr = 0.71;//prandtl number
	double R_gas_constant = 287;//gas constant 

	void timestepCalculator();//calTimeStep
	void maccormackPush();//cal n+1 value by maccormack method
	void convergenceJudgement();//judge whether the residuals convergence
	void tau_xx_Calculator();//calculate tau_xx 
	void tau_xy_Calculator();//calculate tau_xx 
	void tau_yy_Calculator();//calculate tau_xx 
	void q_x_Calculator();
	void q_y_Calculator();
};
std::ostream& operator<<(std::ostream&, Maccormack&);
#endif // !_MACCORMACK_H_

