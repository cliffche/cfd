#include "maccormack.h"
#include <ostream>

//Constructor create all points by mesh qty of x coordinate & y coordinate
Maccormack::Maccormack(size_t mesh_x, size_t mesh_y) : mesh_x(mesh_x), mesh_y(mesh_y) {
	//variable from 'rho' to 'k' is the base variable to be solved	
	std::vector<std::string> params = {
		"rho","u","v","mod_V","p","T","e","mu","k","tau_xx","tau_xy","tau_yy","Et","q_x","q_y","D_rho_x_forward",
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

//计算t=0时刻的条件
//params of t=0.0s
void Maccormack::initial() {

}
//boundary conditions
void Maccormack::boundary() {

}
void Maccormack::timestepCalculator() {

}
//maccormack推进
void Maccormack::maccormackPush() {

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
void Maccormack::pd_rho_t_forward_Calculator() {
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];
			double D_rho_t_forward = -(p_this["u"] * p_this["D_rho_x_forward"] + p_this["rho"] * (p_this["D_u_x_forward"] * p_this["D_u_y_forward"])
				+ p_this["v"] * p_this["D_rho_y_forward"]);
			p_this["D_rho_t_forward"] = D_rho_t_forward;
		}
	}
}
void Maccormack::pd_rho_t_backward_Calculator() {
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];
			double D_rho_t_backward = -(p_this["u"] * p_this["D_rho_x_backward"] + p_this["rho"] * (p_this["D_u_x_backward"] * p_this["D_u_y_backward"])
				+ p_this["v"] * p_this["D_rho_y_backward"]);
			p_this["D_rho_t_backward"] = D_rho_t_backward;
		}
	}
}

void Maccormack::pd_u_t_forward_Calculator() {

}
void Maccormack::pd_u_t_backward_Calculator() {

}
void Maccormack::pd_v_t_forward_Calculator() {

}
void Maccormack::pd_v_t_backward_Calculator() {

}

void Maccormack::basic_pd_Calculator() {
	for (size_t i = 1; i < mesh_x - 1; i++) {
		for (size_t j = 1; j < mesh_y - 1; j++)
		{
			map_point& p_this = points[i][j];
			map_point& p_pre_x = points[i - 1][j];
			map_point& p_pre_y = points[i][j - 1];
			map_point& p_next_x = points[i + 1][j];
			map_point& p_next_y = points[i][j + 1];
			double D_rho_x_forward = (p_next_x["rho"] - p_this["rho"]) / dx;
			p_this["D_rho_x_forward"] = D_rho_x_forward;
			double D_rho_y_forward = (p_next_y["rho"] - p_this["rho"]) / dx;
			p_this["D_rho_y_forward"] = D_rho_x_forward;
			double D_u_x_forward = (p_next_x["u"] - p_this["u"]) / dx;
			p_this["D_u_x_forward"] = D_rho_x_forward;
			double D_u_y_forward = (p_next_y["u"] - p_this["u"]) / dx;
			p_this["D_u_y_forward"] = D_rho_x_forward;
			double D_rho_x_backward = (p_this["rho"] - p_pre_x["rho"]) / dx;
			p_this["D_rho_x_backward"] = D_rho_x_forward;
			double D_rho_y_backward = (p_this["rho"] - p_pre_y["rho"]) / dx;
			p_this["D_rho_y_backward"] = D_rho_x_forward;
			double D_u_x_backward = (p_this["u"] - p_pre_x["u"]) / dx;
			p_this["D_u_x_backward"] = D_rho_x_forward;
			double D_u_y_backward = (p_this["u"] - p_pre_y["u"]) / dx;
			p_this["D_u_y_backward"] = D_rho_x_forward;
		}
	}