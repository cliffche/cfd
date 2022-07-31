#include "nozzle.h"
#include <math.h>
#include <iostream>

Nozzle::Nozzle(size_t point_qty, size_t time_step_total, double x_len, double Ax_coeffi_2, double  Ax_coeffi_1, double Ax_coeffi_0, double gama_, double Courant_)
	:point_qty(point_qty), time_step_total(time_step_total), x_len(x_len), Ax_coeffi_2(Ax_coeffi_2),
	Ax_coeffi_1(Ax_coeffi_1), Ax_coeffi_0(Ax_coeffi_0), gama_(gama_), Courant_(Courant_)
{
	delta_x = x_len / (point_qty - 1);
	delta_x_reciprocal = (point_qty - 1) / x_len;
	init_geometry_parameter();
}

void Nozzle::init_geometry_parameter() {//��ʼ���β���
	size_t i = 0;
	std::vector<std::map<std::string, double>> ::iterator iter = param_Vec.begin();
	while (iter != param_Vec.end())
	{
		double x_value = i * delta_x;
		double A_value = Ax_coeffi_2 * pow(x_value, 2) + Ax_coeffi_1 * x_value + Ax_coeffi_0;
		(*iter)["x_value"] = x_value;
		(*iter)["A"] = A_value;
		i++;
		iter++;
	}
	calc_init = true;//�Ѿ���ɳ�ʼ��
}

void Nozzle::init_calc_parameter(double rhox_coeffi_1, double rhox_coeffi_0, double Tx_coeffi_1, double Tx_coeffi_0, double ux_coeffi_1, double ux_coeffi_0) {//��ʼ����������
	size_t i = 0;
	std::vector<std::map<std::string, double>> ::iterator iter = param_Vec.begin();
	while (iter != param_Vec.end())
	{
		double x_value = (*iter)["x_value"];
		(*iter)["rho"] = rhox_coeffi_1 * x_value + rhox_coeffi_0;
		double T_value = Tx_coeffi_1 * x_value + Tx_coeffi_0;
		(*iter)["T"] = T_value;
		(*iter)["u"] = (ux_coeffi_0 + ux_coeffi_1 * x_value) * (pow(T_value, 0.5));
		i++;
		iter++;
	}
}

void Nozzle::calc_timestep() {
	double temp_value_max = 0;//�м�ֵ ����T+u
	double temp_value;
	std::vector< std::map<std::string, double> > ::iterator iter = param_Vec.begin();
	while (iter != param_Vec.end()) {
		temp_value = pow((*iter)["T"], 0.5) + (*iter)["u"];
		temp_value_max = std::max(temp_value_max, temp_value);
		iter++;
	}
	time_step = delta_x * Courant_ / temp_value_max;
}
void Nozzle::calc_predict() {//����Ԥ�ⲽ
	for (size_t j = 0; j < point_qty - 1; j++) {
		//j=point_qty - 1��ǰ��
		//��Ϊ����ʽ,���еļ����������tʱ�̵�,��next��ʾi+1��		
		double A_this = param_Vec[j]["A"];//A_i_t
		double A_next = param_Vec[j + 1]["A"];//A_i+1_t
		double rho_this = param_Vec[j]["rho"];//rho_i_t
		double rho_next = param_Vec[j + 1]["rho"];//rho_i+1_t
		double T_this = param_Vec[j]["T"];//T_i_t
		double T_next = param_Vec[j + 1]["T"];//T_i+1_t
		double u_this = param_Vec[j]["u"];//u_i_t
		double u_next = param_Vec[j + 1]["u"];//u_i+1_t
		double u_difference_pre = (u_next - u_this) * delta_x_reciprocal;//uǰ��
		double T_difference_pre = (T_next - T_this) * delta_x_reciprocal;//Tǰ��
		double rho_difference_pre = (rho_next - rho_this) * delta_x_reciprocal;//rhoǰ��
		double ln_A_difference_pre = (log(A_next / A_this)) * delta_x_reciprocal;//ln Aǰ��
		//����ƫrhoƫtԤ��ֵ rhoԤ����
		double pd_rho_t_pre = -rho_this * u_difference_pre
			- rho_this * u_this * ln_A_difference_pre
			- u_this * rho_difference_pre;
		double rho_t_pre = rho_this + pd_rho_t_pre * time_step;
		//����ƫuƫtԤ��ֵ uԤ����
		double pd_u_t_pre = -u_this * u_difference_pre
			- (1 / gama_) * (T_difference_pre + T_this / rho_this * rho_difference_pre);
		double u_t_pre = u_this + pd_u_t_pre * time_step;
		//����ƫTƫtԤ��ֵ TԤ����
		double pd_T_t_pre = -u_this * T_difference_pre - (gama_ - 1) * T_this * (u_difference_pre + u_this * ln_A_difference_pre);
		double T_t_pre = T_this + pd_T_t_pre * time_step;
		//����vector<map>��,ȥ����������
		param_Vec[j]["pd_rho_t_pre"] = pd_rho_t_pre;
		param_Vec[j]["rho_t_pre"] = rho_t_pre;
		param_Vec[j]["pd_u_t_pre"] = pd_u_t_pre;
		param_Vec[j]["u_t_pre"] = u_t_pre;
		param_Vec[j]["pd_T_t_pre"] = pd_T_t_pre;
		param_Vec[j]["T_t_pre"] = T_t_pre;
	}
	// rho_0��T_0��֪,Ԥ��ֵ�Ļ���ʵֵ
	param_Vec[0]["T_t_pre"] = 1;
	param_Vec[0]["rho_t_pre"] = 1;
}

void Nozzle::calc_correction() {//���������������ս��
	for (size_t k = 1; k < point_qty - 1; k++) {
		//k=0�޺��,k=max���������
		//���������������ս��������ʽͬԤ�ⲽ
		double pd_rho_t_pre = param_Vec[k]["pd_rho_t_pre"];
		double pd_u_t_pre = param_Vec[k]["pd_u_t_pre"];
		double pd_T_t_pre = param_Vec[k]["pd_T_t_pre"];

		double rho_t_pre_this = param_Vec[k]["rho_t_pre"];
		double rho_t_pre_last = param_Vec[k - 1]["rho_t_pre"];
		double rho_t_pre_difference_cor = (rho_t_pre_this - rho_t_pre_last) * delta_x_reciprocal;//rho���

		double u_t_pre_this = param_Vec[k]["u_t_pre"];
		double u_t_pre_last = param_Vec[k - 1]["u_t_pre"];
		double u_t_pre_difference_cor = (u_t_pre_this - u_t_pre_last) * delta_x_reciprocal;//u���

		double T_t_pre_this = param_Vec[k]["T_t_pre"];
		double T_t_pre_last = param_Vec[k - 1]["T_t_pre"];
		double T_t_pre_difference_cor = (T_t_pre_this - T_t_pre_last) * delta_x_reciprocal;//T���

		double A_this = param_Vec[k]["A"];//A_i_t
		double A_last = param_Vec[k - 1]["A"];//A_i+1_t
		double ln_A_difference_pre = (log(A_this / A_last)) * delta_x_reciprocal;//ln A���
		//��������ֵ
		double pd_rho_t_cor = -rho_t_pre_this * u_t_pre_difference_cor
			- rho_t_pre_this * u_t_pre_this * ln_A_difference_pre
			- u_t_pre_this * rho_t_pre_difference_cor;
		double pd_u_t_cor = -u_t_pre_this * u_t_pre_difference_cor
			- (1 / gama_) * (T_t_pre_difference_cor + (T_t_pre_this / rho_t_pre_this) * rho_t_pre_difference_cor);
		double pd_T_t_cor = -u_t_pre_this * T_t_pre_difference_cor - (gama_ - 1) * T_t_pre_this * (u_t_pre_difference_cor + u_t_pre_this * ln_A_difference_pre);
		//Ԥ����������ƽ��ֵ
		double pd_rho_t_average = (pd_rho_t_cor + pd_rho_t_pre) / 2;
		double pd_u_t_average = (pd_u_t_cor + pd_u_t_pre) / 2;
		double pd_T_t_average = (pd_T_t_cor + pd_T_t_pre) / 2;
		//����rho , u, T
		param_Vec[k]["rho"] += pd_rho_t_average * time_step;
		param_Vec[k]["u"] += pd_u_t_average * time_step;
		param_Vec[k]["T"] += pd_T_t_average * time_step;
	}
	//����0������һ���ֵ rho��T��֪
	param_Vec[0]["u"] = 2 * param_Vec[1]["u"] - param_Vec[2]["u"];
	param_Vec[point_qty - 1]["rho"] = 2 * param_Vec[point_qty - 2]["rho"] - param_Vec[point_qty - 3]["rho"];
	param_Vec[point_qty - 1]["u"] = 2 * param_Vec[point_qty - 2]["u"] - param_Vec[point_qty - 3]["u"];
	param_Vec[point_qty - 1]["T"] = 2 * param_Vec[point_qty - 2]["T"] - param_Vec[point_qty - 3]["T"];
}
void Nozzle::cout_result() {
	std::vector<std::map<std::string, double>> ::iterator it = param_Vec.begin();
	while (it != param_Vec.end()) {
		std::cout << (*it)["x_value"] << "   " << (*it)["A"] << "   " << (*it)["rho"] << "   " << (*it)["u"] << "   " << (*it)["T"] << std::endl;
		it++;
	}
}

void Nozzle::solve() {//����������
	if (!calc_init) {
		std::cout << "������ɳ�ʼ��" << std::endl;
		return;
	}
	else {
		for (size_t i = 0; i < time_step_total; i++) {
			calc_timestep();
			calc_predict();
			calc_correction();
		}
	}
}