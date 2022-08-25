#pragma once
#ifndef _TWO_DIMENSIONAL_EXPLICIT_SOLVER_H_
#define _TWO_DIMENSIONAL_EXPLICIT_SOLVER_H_
#include <map>
#include <vector>
#include <string>
#include <ostream>
using map_point = std::map<std::string, double>; //use map structure to develop the program for code semanticsand development efficiency
//maccormack推进方法,二维完整n-s方程
class Tdes
{
public:
	Tdes(size_t, size_t);
	friend std::ostream& operator<<(std::ostream&, Tdes&);
	void setInitial();//initialization condition
	void setBoundary(double, double, double, double);//boundary conditions
	void solve();
private:
	std::vector<std::vector<map_point>> points;
	size_t total_timestep = 10;
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

	double T_wall = 0;
	double T_infinity = 0;
	double p_infinity = 0;
	double u_infinity = 0;

	bool hasSetIC = false;
	bool hasSetBC = false;

	void timestepCalculator();//calTimeStep
	void maccormackPush();//cal time n+1 value by explictit method(maccormack)
	void interpolationBundary();//bundaries determined by inside points
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
	//TODOs
	void tau_xx_Calculator();//calculate tau_xx 
	void tau_xy_Calculator();//calculate tau_xx 
	void tau_yy_Calculator();//calculate tau_xx 
	void q_x_Calculator();
	void q_y_Calculator();
};
std::ostream& operator<<(std::ostream&, Tdes&);
#endif // !_TWO_DIMENSIONAL_EXPLICIT_SOLVER_H_

