#pragma once
#ifndef _NOZZLE_H_
#define _NOZZLE_H_
#include <map>
#include <vector>
#include <string>
class Nozzle
{
public:
	Nozzle(size_t, size_t, double, double, double, double, double, double);// 网格数量, 总时间步, x_len, A(x)2次项,1次项,常数项,gama,Courant数
	void init_geometry_parameter();//
	void init_calc_parameter(double, double, double, double, double, double);//calculate initial value
	void calc_timestep();//calculate the single step time
	void calc_predict();//prediction step calculation
	void calc_correction();//correction step calculation
	void solve();//solve entry
	void cout_result();//cout matrix
private:
	const size_t point_qty;//网格节点数
	const double x_len; //x长度
	double delta_x; //x长度
	double delta_x_reciprocal;
	const double Courant_;//Courant数
	const size_t time_step_total;//总时间步
	const double gama_; //γ 	
	const double Ax_coeffi_2;
	const double Ax_coeffi_1;
	const double Ax_coeffi_0;
	double time_step = 0.0;//步长
	bool calc_init = false;//已经完成计算条件初始化
	std::map<std::string, double> point_ = {
		{"x_value",0},//x方向的值
		{"A",0},//无量纲宽度
		{"rho",0}, //无量纲密度
		{"T",0},//无量纲温度	
		{"u",0},//无量纲速度
		{"pd_rho_t_pre",0},//偏rho偏t预测值
		{"pd_u_t_pre",0},//偏u偏t预测值
		{"pd_T_t_pre",0},//偏T偏t预测值
		{"rho_t_pre",0},//rho预测量
		{"u_t_pre",0},//u预测量
		{"T_t_pre",0},//T预测量
		{"pd_rho_t_cor",0},//偏rho偏t修正值
		{"pd_u_t_cor",0},//偏u偏t修正值
		{"pd_T_t_cor",0},//偏T偏t修正值
	};
	std::vector < std::map<std::string, double> > param_Vec = { point_qty, point_ };
};
#endif // !_NOZZLE_H_
