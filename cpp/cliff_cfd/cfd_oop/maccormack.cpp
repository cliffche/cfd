#include "maccormack.h"
#include <ostream>

//Constructor create all points by mesh qty of x coordinate & y coordinate
Maccormack::Maccormack(size_t mesh_x, size_t mesh_y) {
	//variable from 'rho' to 'k' is the base variable to be solved	
	std::vector<std::string> params = { "rho","u","v","mod_V","p","T","e","mu","k","tau_xx","tau_xy","tau_yy","Et","q_x","q_y" };
	std::vector<std::string>::const_iterator itr = params.begin();
	map_point p0;
	while (itr != params.end()) {
		p0.insert({ *itr, 0.0 });
		itr++;
	}
	points = { mesh_x ,{mesh_y , p0} };
}

//output all points info by reload operator<<
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

//params of t=0.0s
void Maccormack::initial() {

}
//boundary conditions
void Maccormack::boundary() {

}
void Maccormack::timestepCalculator() {

}
void Maccormack::maccormackPush() {

}
void Maccormack::convergenceJudgement() {

}
void Maccormack::tau_xx_Calculator() {

}
void Maccormack::tau_xy_Calculator() {

}
void Maccormack::tau_yy_Calculator() {

}
