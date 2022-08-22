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
	void setInitial();//initialization condition
	void setBoundary(double, double, double);//boundary conditions
private:
	std::vector<std::vector<map_point>> points;
	size_t total_timestep = 1000;
	size_t mesh_x, mesh_y;
	double dx = 0.1;
	double dy = 0.1;
	double courant = 0.7;//courant number
	double gama = 1.4;//gama default air
	double Cv = 4.18;//default cv
	double pr = 0.71;//prandtl number
	double R_gas_constant = 287.0;//gas constant 
	double T_0 = 298.15;
	double mu_0 = 1;
	double delta_t = 1;
	double delta_x = 1;
	double delta_y = 1;
	void timestepCalculator();//calTimeStep
	void maccormackPush();//cal n+1 value by maccormack method
	void convergenceJudgement();//judge whether the residuals convergence
	inline double pd_rho_t_forward_Calculator(map_point&);//calculate front pd rho/pd t
	inline double pd_rho_t_backward_Calculator(map_point&);//calculate front pd rho/pd t	
	inline double pd_u_t_forward_Calculator(map_point&);//
	inline double pd_u_t_backward_Calculator(map_point&);//
	inline double pd_v_t_forward_Calculator(map_point&);//
	inline double pd_v_t_backward_Calculator(map_point&);//
	inline double pd_e_t_forward_Calculator(map_point&);//
	inline double pd_e_t_backward_Calculator(map_point&);//
	void basic_pd_forward_Calculator();
	void basic_pd_backward_Calculator();//
	//TODOS
	void tau_xx_Calculator();//calculate tau_xx 
	void tau_xy_Calculator();//calculate tau_xx 
	void tau_yy_Calculator();//calculate tau_xx 
	void q_x_Calculator();
	void q_y_Calculator();
};
std::ostream& operator<<(std::ostream&, Maccormack&);
#endif // !_MACCORMACK_H_

