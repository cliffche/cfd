#include "two_dimensional_explicit_solver.h"
#include <iostream>
#include "math.h"
//Constructor create all points by mesh qty of x coordinate & y coordinate
Tdes::Tdes(size_t mesh_x, size_t mesh_y) : mesh_x(mesh_x), mesh_y(mesh_y) {
	//variable from 'rho' to 'e' is the base variable to be solved	
	std::vector<std::string> params = {
		"rho","u","v","e","p","T","mod_V","mu","k","D_rho_x_forward",
		"D_rho_y_forward","D_u_x_forward","D_u_y_forward","D_rho_x_backward","D_rho_y_backward","D_u_x_backward","D_u_y_backward",
		"D_rho_t_forward","D_rho_t_backward"
	};//"tau_xx","tau_xy","tau_yy","Et","q_x","q_y",
	std::vector<std::string>::const_iterator itr = params.begin();
	map_point p0;
	while (itr != params.end()) {
		p0.insert({ *itr, 0.0 });
		itr++;
	}
	points = { mesh_x ,{mesh_y , p0} };
}

//print all points info by overload operator<<
std::ostream& operator<<(std::ostream& os, Tdes& mac) {
	int y_decrease = ceil(static_cast<double>(mac.mesh_y) / mac.maxOutputPoint_y);
	int x_increase = ceil(static_cast<double>(mac.mesh_x) / mac.maxOutputPoint_x);
	std::vector<std::string> li = { "u","e","T" };
	std::vector<std::string>::const_iterator itr = li.begin();
	while (itr != li.end())
	{
		for (int j = mac.mesh_y - 1; j >= 0; j -= y_decrease) {
			os << j << "\t";
			for (size_t i = 0; i <= mac.mesh_x - 1; i += x_increase) {
				os << mac.points[i][j][*itr] << "\t";
			}
			os << std::endl;
		}
		os << *itr << "\t";

		for (size_t i = 0; i < mac.mesh_x; i += x_increase) {
			os << i << "\t";
		}
		os << "\n" << std::endl;
		itr++;
	}
	os << std::endl;
	return os;
}

void Tdes::solve(Tdes& tdes) {
	if (!hasSetBC) {
		std::cout << "has not set boundry conditions" << std::endl;
		return;
	}
	size_t time_step_now = 0;
	while (time_step_now < total_timestep) {
		maccormackPush();
		interpolationBundary();
		std::cout << tdes << std::endl;
		time_step_now++;
	}
}

//计算t=0时刻的条件
//params of t=0.0s
void Tdes::setInitial() {
	double e_infinity = Cv * T_infinity;
	double rho_infinity = p_infinity / (R_gas_constant * T_infinity);
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++) {
			points[i][j]["u"] = u_infinity;
			points[i][j]["v"] = 0;
			points[i][j]["rho"] = rho_infinity;
			points[i][j]["p"] = p_infinity;
			points[i][j]["T"] = T_infinity;
			points[i][j]["e"] = e_infinity;
		}
	}
	std::cout << "Set init" << std::endl;
	std::cout << *this << std::endl;
	//hasSetIC = true;
}
//boundary conditions
void Tdes::setBoundary(double T_wall, double T_infinity, double p_infinity, double u_infinity) {

	double e_wall = Cv * T_wall;
	double e_infinity = Cv * T_infinity;
	double rho_infinity = p_infinity / (R_gas_constant * T_infinity);
	this->T_wall = T_wall;
	this->T_infinity = T_infinity;
	this->p_infinity = p_infinity;
	this->u_infinity = u_infinity;
	setInitial();
	for (size_t i = 1; i <= mesh_x - 1; i++) {
		//bottom bundary
		points[i][0]["u"] = 0;
		points[i][0]["v"] = 0;
		points[i][0]["e"] = e_wall;
		points[i][0]["T"] = T_wall;
		//points[i][0]["p"] = e_wall;
		//points[i][0]["rho"] = e_wall;

		//up bundary
		points[i][mesh_x - 1]["u"] = u_infinity;
		points[i][mesh_x - 1]["v"] = 0;
		points[i][mesh_x - 1]["e"] = e_infinity;
		points[i][mesh_x - 1]["p"] = p_infinity;
		points[i][mesh_x - 1]["rho"] = rho_infinity;
		points[i][mesh_x - 1]["T"] = T_infinity;
	}
	for (size_t j = 1; j <= mesh_y - 1; j++) {
		//left bundary
		points[0][j]["u"] = u_infinity;
		points[0][j]["v"] = 0;
		points[0][j]["e"] = e_infinity;
		points[0][j]["p"] = p_infinity;
		points[0][j]["rho"] = rho_infinity;
		points[0][j]["T"] = T_infinity;
	}
	points[0][0]["u"] = 0;
	points[0][0]["v"] = 0;
	points[0][0]["rho"] = rho_infinity;
	points[0][0]["e"] = e_infinity;
	points[0][0]["T"] = T_infinity;
	points[0][0]["p"] = p_infinity;

	interpolationBundary();
	std::cout << "Set boundary" << std::endl;
	std::cout << *this << std::endl;
	hasSetBC = true;
}
//interpolate to get boundaries
void Tdes::interpolationBundary() {
	for (size_t i = 1; i < mesh_x - 1; i++) {

		points[i][0]["p"] = 2.0 * points[i][1]["p"] - points[i][2]["p"];
		points[i][0]["rho"] = 2.0 * points[i][1]["rho"] - points[i][2]["rho"];
		points[i][0]["e"] = 2.0 * points[i][1]["e"] - points[i][2]["e"];
	}
	for (size_t j = 1; j < mesh_y - 1; j++) {
		points[mesh_x - 1][j]["p"] = 2.0 * points[mesh_x - 2][j]["p"] - points[mesh_x - 3][j]["p"];
		points[mesh_x - 1][j]["u"] = 2.0 * points[mesh_x - 2][j]["u"] - points[mesh_x - 3][j]["u"];
		points[mesh_x - 1][j]["v"] = 2.0 * points[mesh_x - 2][j]["v"] - points[mesh_x - 3][j]["v"];
		points[mesh_x - 1][j]["rho"] = 2.0 * points[mesh_x - 2][j]["rho"] - points[mesh_x - 3][j]["rho"];
		points[mesh_x - 1][j]["e"] = 2.0 * points[mesh_x - 2][j]["e"] - points[mesh_x - 3][j]["e"];
		points[mesh_x - 1][j]["T"] = 2.0 * points[mesh_x - 2][j]["T"] - points[mesh_x - 3][j]["T"];
	}
}

void Tdes::timestepCalculator() {
	double v_ = 0;
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];
			double v_temp = pow(mu_0, 2) * pow(p_this["T"] / T_0, 3) * pow((T_0 + 110) / (p_this["T"] + 110), 2) * gama * pr * 4.0 / (3.0 * p_this["rho"]);
			v_ = std::max(v_temp, v_);
		}
	}
	//std::cout << " v_" << v_ << std::endl;
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];
			double a_ = std::sqrt(gama * R_gas_constant * p_this["T"]);
			double temp_ = 1.0 / pow(dx, 2) + 1.0 / pow(dy, 2);
			double delta_t_temp = 1.0 / (std::abs(p_this["u"]) / dx + std::abs(p_this["v"]) / dy + a_ * std::sqrt(temp_)
				+ 2 * v_ * (temp_));
			this->delta_t = std::min(courant * delta_t_temp, this->delta_t);
		}
	}
	std::cout << "delta_t" << delta_t << std::endl;
}
//maccormack推进
void Tdes::maccormackPush() {
	//predict step
	timestepCalculator();
	basic_pd_forward_Calculator();
	for (size_t i = 0; i < mesh_x - 1; i++) {
		for (size_t j = 0; j < mesh_y - 1; j++)
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

			p_this["T"] = p_this["e"] / Cv;
			p_this["p"] = R_gas_constant * p_this["T"] * p_this["rho"];
		}
	}
}

//偏rho偏t前差
double Tdes::pd_rho_t_forward_Calculator(map_point& p_this) {
	double D_rho_t_forward = -(p_this["u"] * p_this["D_rho_x_forward"] + p_this["rho"] * (p_this["D_u_x_forward"] * p_this["D_u_y_forward"])
		+ p_this["v"] * p_this["D_rho_y_forward"]);
	return D_rho_t_forward;
}
double Tdes::pd_rho_t_backward_Calculator(map_point& p_this) {
	double D_rho_t_backward = -(p_this["u"] * p_this["D_rho_x_backward"] + p_this["rho"] * (p_this["D_u_x_backward"] * p_this["D_u_y_backward"])
		+ p_this["v"] * p_this["D_rho_y_backward"]);
	return D_rho_t_backward;
}

//temporarily program invis equation 
double Tdes::pd_u_t_forward_Calculator(map_point& p_this) {
	double D_u_t_forward = -(p_this["u"] * p_this["D_u_x_forward"] + p_this["v"] * p_this["D_u_y_forward"]
		+ 1.0 / p_this["rho"] * p_this["D_p_x_forward"]);
	return D_u_t_forward;
}
double Tdes::pd_u_t_backward_Calculator(map_point& p_this) {
	double D_u_t_backward = -(p_this["u"] * p_this["D_u_x_backward"] + p_this["v"] * p_this["D_u_y_backward"]
		+ 1.0 / p_this["rho"] * p_this["D_p_x_backward"]);
	return D_u_t_backward;

}
double Tdes::pd_v_t_forward_Calculator(map_point& p_this) {
	double D_v_t_forward = -(p_this["v"] * p_this["D_v_x_forward"] + p_this["u"] * p_this["D_v_y_forward"]
		+ 1.0 / p_this["rho"] * p_this["D_p_y_forward"]);
	return D_v_t_forward;
}

double Tdes::pd_v_t_backward_Calculator(map_point& p_this) {
	double D_v_t_backward = -(p_this["v"] * p_this["D_v_x_backward"] + p_this["u"] * p_this["D_v_y_backward"]
		+ 1.0 / p_this["rho"] * p_this["D_p_y_backward"]);
	return D_v_t_backward;
}

double Tdes::pd_e_t_forward_Calculator(map_point& p_this) {
	double D_e_t_forward = -(p_this["p"] / p_this["rho"] * (p_this["D_u_x_forward"] + p_this["D_v_y_forward"]) + p_this["u"] * p_this["D_e_x_forward"]
		+ p_this["v"] * p_this["D_e_y_forward"]);
	return D_e_t_forward;
}

double Tdes::pd_e_t_backward_Calculator(map_point& p_this) {

	double D_e_t_backward = -(p_this["p"] / p_this["rho"] * (p_this["D_u_x_backward"] + p_this["D_v_y_backward"]) + p_this["u"] * p_this["D_e_x_backward"]
		+ p_this["v"] * p_this["D_e_y_backward"]);
	return D_e_t_backward;
}

void Tdes::basic_pd_forward_Calculator() {
	for (size_t i = 0; i < mesh_x - 1; i++) {
		for (size_t j = 0; j < mesh_y - 1; j++)
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

void Tdes::basic_pd_backward_Calculator() {
	for (size_t i = 1; i <= mesh_x - 1; i++) {
		for (size_t j = 1; j <= mesh_y - 1; j++)
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
//todos
//收敛判断 
void Tdes::convergenceJudgement() {
}
//τxx计算
void Tdes::tau_xx_Calculator() {
}
//τxy计算
void Tdes::tau_xy_Calculator() {
}
//τyy计算
void Tdes::tau_yy_Calculator() {
}
void Tdes::q_x_Calculator() {
}
void Tdes::q_y_Calculator() {
}