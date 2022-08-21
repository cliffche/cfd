#pragma once
#ifndef _NOZZLE_H_
#define _NOZZLE_H_
#include <map>
#include <vector>
#include <string>
class Nozzle
{
public:
	Nozzle(size_t, size_t, double, double, double, double, double, double);// ��������, ��ʱ�䲽, x_len, A(x)2����,1����,������,gama,Courant��
	void init_geometry_parameter();//
	void init_calc_parameter(double, double, double, double, double, double);//calculate initial value
	void calc_timestep();//calculate the single step time
	void calc_predict();//prediction step calculation
	void calc_correction();//correction step calculation
	void solve();//solve entry
	void cout_result();//cout matrix
private:
	const size_t point_qty;//����ڵ���
	const double x_len; //x����
	double delta_x; //x����
	double delta_x_reciprocal;
	const double Courant_;//Courant��
	const size_t time_step_total;//��ʱ�䲽
	const double gama_; //�� 	
	const double Ax_coeffi_2;
	const double Ax_coeffi_1;
	const double Ax_coeffi_0;
	double time_step = 0.0;//����
	bool calc_init = false;//�Ѿ���ɼ���������ʼ��
	std::map<std::string, double> point_ = {
		{"x_value",0},//x�����ֵ
		{"A",0},//�����ٿ��
		{"rho",0}, //�������ܶ�
		{"T",0},//�������¶�	
		{"u",0},//�������ٶ�
		{"pd_rho_t_pre",0},//ƫrhoƫtԤ��ֵ
		{"pd_u_t_pre",0},//ƫuƫtԤ��ֵ
		{"pd_T_t_pre",0},//ƫTƫtԤ��ֵ
		{"rho_t_pre",0},//rhoԤ����
		{"u_t_pre",0},//uԤ����
		{"T_t_pre",0},//TԤ����
		{"pd_rho_t_cor",0},//ƫrhoƫt����ֵ
		{"pd_u_t_cor",0},//ƫuƫt����ֵ
		{"pd_T_t_cor",0},//ƫTƫt����ֵ
	};
	std::vector < std::map<std::string, double> > param_Vec = { point_qty, point_ };
};
#endif // !_NOZZLE_H_
