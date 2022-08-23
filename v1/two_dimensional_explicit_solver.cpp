#include "two_dimensional_explicit_solver.h"
#include <iostream>
#include "math.h"
//Constructor create all points by mesh qty of x coordinate & y coordinate
Maccormack::Maccormack(size_t mesh_x, size_t mesh_y) : mesh_x(mesh_x), mesh_y(mesh_y) {
	//variable from 'rho' to 'e' is the base variable to be solved	
	std::vector<std::string> params = {
		"rho","u","v","e","mod_V","p","T","mu","k","tau_xx","tau_xy","tau_yy","Et","q_x","q_y","D_rho_x_forward",
		"D_rho_y_forward","D_u_x_forward","D_u_y_forward","D_rho_x_backward","D_rho_y_backward","D_u_x_backward","D_u_y_backward",
		"D_rho_t_forward","D_rho_t_backward"
	};
	std::vector<std::string>::const_iterator itr = params.begin();
	map_point p0;
	while (itr != params.end()) {
		p0.insert({ *itr, 0.0 });
		itr++;
	}
	points = { mesh_x ,{mesh_y , p0} };
}

//print all points info by overload operator<<
std::ostream& operator<<(std::ostream& os, Maccormack& mac) {
	os << "(x,y)" << "\t";
	std::map<std::string, double>::iterator it = mac.points[0][0].begin();
	while (it != mac.points[0][0].end())
	{
		os << it->first << "\t";
		++it;
	}
	os << std::endl;
	for (size_t i = 0; i < mac.points.size(); i++) {
		for (size_t j = 0; j < mac.points[i].size(); j++) {
			std::map<std::string, double>::iterator itr = mac.points[i][j].begin();
			os << i << "," << j << "\t";
			while (itr != mac.points[i][j].end())
			{
				os << itr->second << "\t";
				++itr;
			}
			os << std::endl;
		}
		os << std::endl;
	}
	return os;
}

void Maccormack::master() {
	if (!hasSetBC || !hasSetIC) {
		std::cout << "has not set boundry conditions or initial conditions" << std::endl;
		return;
	}

}

//计算t=0时刻的条件
//params of t=0.0s
void Maccormack::setInitial() {
	hasSetIC = true;
}
//boundary conditions
void Maccormack::setBoundary(double T_wall, double T_infinity, double rho_infinity) {
	double e_wall = Cv * T_wall;
	double e_infinity = Cv * T_infinity;
	double p_infinity = rho_infinity * (R_gas_constant * T_infinity);
	for (size_t i = 1; i < mesh_y - 1; i++) {
		points[0][i]["u"] = 0;
		points[0][i]["v"] = 0;
		points[0][i]["e"] = e_wall;

		points[mesh_x - 1][i]["u"] = 0;
		points[mesh_x - 1][i]["v"] = 0;
		points[mesh_x - 1][i]["e"] = e_infinity;
		points[mesh_x - 1][i]["p"] = p_infinity;
		points[mesh_x - 1][i]["rho"] = rho_infinity;


	}
	hasSetBC = true;
}
void Maccormack::timestepCalculator() {
	double v_ = 0;
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];
			double v_temp = mu_0 * pow(p_this["T"] / T_0, 3) * pow((T_0 + 110) / (p_this["T"] + 110), 2) * gama * pr;
			v_ = std::max(v_temp, v_);
		}
	}
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];
			double a_ = std::sqrt(gama * R_gas_constant * p_this["T"]);
			double temp_ = 1.0 / pow(delta_x, 2) + 1.0 / pow(delta_y, 2);
			double delta_t_temp = 1.0 / (std::abs(p_this["u"]) / delta_x + std::abs(p_this["v"]) / delta_y) + a_ * std::sqrt(temp_)
				+ 2 * v_ * (temp_);
			delta_t = std::min(courant * delta_t_temp, delta_t);
		}
	}
}
//maccormack推进
void Maccormack::maccormackPush() {
	//predict step
	timestepCalculator();
	basic_pd_forward_Calculator();
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];

			double pd_rho_t_forward = pd_rho_t_forward_Calculator(p_this);
			double rho_predict = p_this["rho"] + pd_rho_t_forward * delta_t;
			p_this["rho_predict"] = rho_predict;
			p_this["pd_rho_t_forward"] = pd_rho_t_forward;

			double pd_u_t_forward = pd_u_t_forward_Calculator(p_this);
			double u_predict = p_this["u"] + pd_u_t_forward * delta_t;
			p_this["u_predict"] = u_predict;
			p_this["pd_u_t_forward"] = pd_u_t_forward;

			double pd_v_t_forward = pd_v_t_forward_Calculator(p_this);
			double v_predict = p_this["v"] + pd_v_t_forward * delta_t;
			p_this["v_predict"] = v_predict;
			p_this["pd_v_t_forward"] = pd_v_t_forward;

			double pd_e_t_forward = pd_e_t_forward_Calculator(p_this);
			double e_predict = p_this["e"] + pd_e_t_forward * delta_t;
			p_this["e_predict"] = e_predict;
			p_this["pd_e_t_forward"] = pd_e_t_forward;
		}
	}
	basic_pd_backward_Calculator();
	//correct step
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];
			double pd_rho_t_backward = pd_rho_t_backward_Calculator(p_this);
			p_this["rho"] += 0.5 * (p_this["pd_rho_t_forward"] + pd_rho_t_backward) * delta_t;

			double pd_u_t_backward = pd_u_t_backward_Calculator(p_this);
			p_this["u"] += 0.5 * (p_this["pd_u_t_forward"] + pd_u_t_backward) * delta_t;

			double pd_v_t_backward = pd_v_t_backward_Calculator(p_this);
			p_this["v"] += 0.5 * (p_this["pd_v_t_forward"] + pd_v_t_backward) * delta_t;

			double pd_e_t_backward = pd_e_t_backward_Calculator(p_this);
			p_this["e"] += 0.5 * (p_this["pd_e_t_forward"] + pd_e_t_backward) * delta_t;
		}
	}
}
//收敛判断
void Maccormack::convergenceJudgement() {
}

//τxx计算
void Maccormack::tau_xx_Calculator() {
}
//τxy计算
void Maccormack::tau_xy_Calculator() {
}
//τyy计算
void Maccormack::tau_yy_Calculator() {
}
void Maccormack::q_x_Calculator() {
}
void Maccormack::q_y_Calculator() {
}
//偏rho偏t前差
double Maccormack::pd_rho_t_forward_Calculator(map_point& p_this) {
	double D_rho_t_forward = -(p_this["u"] * p_this["D_rho_x_forward"] + p_this["rho"] * (p_this["D_u_x_forward"] * p_this["D_u_y_forward"])
		+ p_this["v"] * p_this["D_rho_y_forward"]);
	return D_rho_t_forward;
}
double Maccormack::pd_rho_t_backward_Calculator(map_point& p_this) {
	double D_rho_t_backward = -(p_this["u"] * p_this["D_rho_x_backward"] + p_this["rho"] * (p_this["D_u_x_backward"] * p_this["D_u_y_backward"])
		+ p_this["v"] * p_this["D_rho_y_backward"]);
	return D_rho_t_backward;
}

//temporarily program invis equation 
double Maccormack::pd_u_t_forward_Calculator(map_point& p_this) {
	double D_u_t_forward = -(p_this["u"] * p_this["D_u_x_forward"] + p_this["v"] * p_this["D_u_y_forward"]
		+ 1.0 / p_this["rho"] * p_this["D_p_x_forward"]);
	return D_u_t_forward;
}
double Maccormack::pd_u_t_backward_Calculator(map_point& p_this) {
	double D_u_t_backward = -(p_this["u"] * p_this["D_u_x_backward"] + p_this["v"] * p_this["D_u_y_backward"]
		+ 1.0 / p_this["rho"] * p_this["D_p_x_backward"]);
	return D_u_t_backward;

}
double Maccormack::pd_v_t_forward_Calculator(map_point& p_this) {
	double D_v_t_forward = -(p_this["v"] * p_this["D_v_x_forward"] + p_this["u"] * p_this["D_v_y_forward"]
		+ 1.0 / p_this["rho"] * p_this["D_p_y_forward"]);
	return D_v_t_forward;
}

double Maccormack::pd_v_t_backward_Calculator(map_point& p_this) {
	double D_v_t_backward = -(p_this["v"] * p_this["D_v_x_backward"] + p_this["u"] * p_this["D_v_y_backward"]
		+ 1.0 / p_this["rho"] * p_this["D_p_y_backward"]);
	return D_v_t_backward;
}

double Maccormack::pd_e_t_forward_Calculator(map_point& p_this) {
	double D_e_t_forward = -(p_this["p"] / p_this["rho"] * (p_this["D_u_x_forward"] + p_this["D_v_y_forward"]) + p_this["u"] * p_this["D_e_x_forward"]
		+ p_this["v"] * p_this["D_e_y_forward"]);
	return D_e_t_forward;
}

double Maccormack::pd_e_t_backward_Calculator(map_point& p_this) {

	double D_e_t_backward = -(p_this["p"] / p_this["rho"] * (p_this["D_u_x_backward"] + p_this["D_v_y_backward"]) + p_this["u"] * p_this["D_e_x_backward"]
		+ p_this["v"] * p_this["D_e_y_backward"]);
	return D_e_t_backward;
}

void Maccormack::basic_pd_forward_Calculator() {
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];
			map_point& p_next_x = points[i + 1][j];
			map_point& p_next_y = points[i][j + 1];
			double D_rho_x_forward = (p_next_x["rho"] - p_this["rho"]) / dx;
			p_this["D_rho_x_forward"] = D_rho_x_forward;
			double D_rho_y_forward = (p_next_y["rho"] - p_this["rho"]) / dy;
			p_this["D_rho_y_forward"] = D_rho_y_forward;

			double D_u_x_forward = (p_next_x["u"] - p_this["u"]) / dx;
			p_this["D_u_x_forward"] = D_u_x_forward;
			double D_u_y_forward = (p_next_y["u"] - p_this["u"]) / dy;
			p_this["D_u_y_forward"] = D_u_y_forward;

			double D_p_x_forward = (p_next_x["p"] - p_this["p"]) / dx;
			p_this["D_p_x_forward"] = D_p_x_forward;
			double D_p_y_forward = (p_next_y["p"] - p_this["p"]) / dy;
			p_this["D_p_y_forward"] = D_p_y_forward;

			double D_e_x_forward = (p_next_x["e"] - p_this["e"]) / dx;
			p_this["D_e_x_forward"] = D_e_x_forward;
			double D_e_y_forward = (p_next_y["e"] - p_this["e"]) / dy;
			p_this["D_e_y_forward"] = D_e_y_forward;
		}
	}
}

void Maccormack::basic_pd_backward_Calculator() {
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];
			map_point& p_pre_x = points[i - 1][j];
			map_point& p_pre_y = points[i][j - 1];

			double D_rho_x_backward = (p_this["rho_predict"] - p_pre_x["rho_predict"]) / dx;
			p_this["D_rho_x_backward"] = D_rho_x_backward;
			double D_rho_y_backward = (p_this["rho_predict"] - p_pre_y["rho_predict"]) / dy;
			p_this["D_rho_y_backward"] = D_rho_y_backward;

			double D_u_x_backward = (p_this["u_predict"] - p_pre_x["u_predict"]) / dx;
			p_this["D_u_x_backward"] = D_u_x_backward;
			double D_u_y_backward = (p_this["u_predict"] - p_pre_y["u_predict"]) / dy;
			p_this["D_u_y_backward"] = D_u_y_backward;

			double D_v_x_backward = (p_this["v_predict"] - p_pre_x["p_predict"]) / dx;
			p_this["D_v_x_backward"] = D_v_x_backward;
			double D_v_y_backward = (p_this["v_predict"] - p_pre_y["p_predict"]) / dy;
			p_this["D_v_y_backward"] = D_v_y_backward;

			double D_e_x_backward = (p_this["e_predict"] - p_pre_x["e_predict"]) / dx;
			p_this["D_e_x_backward"] = D_e_x_backward;
			double D_e_y_backward = (p_this["e_predict"] - p_pre_y["e_predict"]) / dy;
			p_this["D_e_y_backward"] = D_e_y_backward;
		}
	}
}