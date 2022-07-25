#include <iostream>
#include <math.h>
#include <map>
#include <array>
#include <vector>

using namespace std;
//void nozzle_solver(int point_x) {
//
//	double A;//�����ٿ��
//	double rho;//�������ܶ�
//	double T;//�������¶�	
//	double u;//�������ٶ�
//	double pd_rho_t_pre;//ƫrhoƫtԤ��ֵ
//	double pd_u_t_pre;//ƫuƫtԤ��ֵ
//	double pd_T_t_pre;//ƫTƫtԤ��ֵ
//	double rho_t_pre;//rhoԤ����
//	double u_t_pre;//uԤ����
//	double T_t_pre;//TԤ����
//	double pd_rho_t_cor;//ƫrhoƫt����ֵ
//	double pd_u_t_cor;//ƫuƫt����ֵ
//	double pd_T_t_cor;//ƫTƫt����ֵ
//}

//A��x��ӳ���ϵ
double calculate_A_value(double x) {
	return 1 + 2.2 * (pow((x - 1.5), 2));
}

//����ʱ�䲽,ȡcount������0.5
double time_step(vector < map<string, double> >* vec, double delta_x, double Courant_value) {
	double temp_value_max = 0;//�м�ֵ ����T+u
	double temp_value;
	vector< map<string, double> > ::iterator it = vec->begin();
	while (it != vec->end()) {
		temp_value = pow((*it)["T"], 0.5) + (*it)["u"];
		temp_value_max = max(temp_value_max, temp_value);
		it++;
	}
	return delta_x * Courant_value / temp_value_max;
}

//cout����
void cout_array_map(vector<map<string, double>>* vec) {
	vector<map<string, double>> ::iterator it = vec->begin();
	while (it != vec->end()) {
		cout << (*it)["x_value"] << "   " << (*it)["A"] << "   " << (*it)["rho"] << "   " << (*it)["u"] << "   " << (*it)["T"] << endl;
		it++;
	}
}

void nozzle_init() {
	//�������ݽṹ�ͳ�ʼ����
	const size_t point_x = 31;//����ڵ���
	double x_len = 3; //x����
	double delta_x = 0.1; //x����
	double delta_x_reciprocal = 1 / delta_x;
	double Courant_value = 0.5;//Courant��
	size_t time_step_total = 20;//��ʱ�䲽
	double gama_air = 1.4; //�����Ħ� 1.4	
	map<string, double> point_0 = {
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
	//������ĳ�ʼ�����ͱ߽�����
	// �߽�rho0 = 1 T0 = 1
	// ��ʼrho = 1-0.3146x T = 1-0.2314x u = (0.1+1.09x)*(T^1/2)
	//��ʼ�����ݽṹ vector��װmap�����ݽṹ,���廯���
	vector < map<string, double> > param_Vec(point_x, point_0);
	//����ÿһ���A��x_value
	vector< map<string, double> > ::iterator it = param_Vec.begin();
	int i = 0;
	while (it != param_Vec.end())
	{
		double x_value = i * delta_x;
		(*it)["x_value"] = x_value;
		(*it)["A"] = calculate_A_value(x_value);
		(*it)["rho"] = 1 - 0.3146 * x_value;
		double T_value = 1 - 0.2314 * x_value;
		(*it)["T"] = T_value;
		(*it)["u"] = (0.1 + 1.09 * x_value) * (pow(T_value, 0.5));
		i++;
		it++;
	}
	for (int t_i = 0; t_i < time_step_total; t_i++) {
		//����delta_t
		cout << t_i << endl;
		double t_step = time_step(&param_Vec, delta_x, Courant_value);
		cout << "t_step= :" << t_step << endl;
		for (size_t j = 0; j < point_x - 1; j++) {//j=30��ǰ��
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
			double rho_t_pre = rho_this + pd_rho_t_pre * t_step;
			//����ƫuƫtԤ��ֵ uԤ����
			double pd_u_t_pre = -u_this * u_difference_pre
				- (1 / gama_air) * (T_difference_pre + T_this / rho_this * rho_difference_pre);
			double u_t_pre = u_this + pd_u_t_pre * t_step;
			//����ƫTƫtԤ��ֵ TԤ����
			double pd_T_t_pre = -u_this * T_difference_pre - (gama_air - 1) * T_this * (u_difference_pre + u_this * ln_A_difference_pre);
			double T_t_pre = T_this + pd_T_t_pre * t_step;
			//����vector<map>��,ȥ����������
			param_Vec[j]["pd_rho_t_pre"] = pd_rho_t_pre;
			param_Vec[j]["rho_t_pre"] = rho_t_pre;
			param_Vec[j]["pd_u_t_pre"] = pd_u_t_pre;
			param_Vec[j]["u_t_pre"] = u_t_pre;
			param_Vec[j]["pd_T_t_pre"] = pd_T_t_pre;
			param_Vec[j]["T_t_pre"] = T_t_pre;
		}
		// rho��T��֪,Ԥ��ֵ�Ļ���ʵֵ
		param_Vec[0]["T_t_pre"] = 1;
		param_Vec[0]["rho_t_pre"] = 1;
		for (size_t k = 1; k < point_x - 1; k++) {//k=0�޺��,k=max���������
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
				- (1 / gama_air) * (T_t_pre_difference_cor + (T_t_pre_this / rho_t_pre_this) * rho_t_pre_difference_cor);
			double pd_T_t_cor = -u_t_pre_this * T_t_pre_difference_cor - (gama_air - 1) * T_t_pre_this * (u_t_pre_difference_cor + u_t_pre_this * ln_A_difference_pre);
			//Ԥ����������ƽ��ֵ
			double pd_rho_t_average = (pd_rho_t_cor + pd_rho_t_pre) / 2;
			double pd_u_t_average = (pd_u_t_cor + pd_u_t_pre) / 2;
			double pd_T_t_average = (pd_T_t_cor + pd_T_t_pre) / 2;
			//����rho , u, T
			param_Vec[k]["rho"] += pd_rho_t_average * t_step;
			param_Vec[k]["u"] += pd_u_t_average * t_step;
			param_Vec[k]["T"] += pd_T_t_average * t_step;
		}
		//����0������һ���ֵ rho��T��֪
		param_Vec[0]["u"] = 2 * param_Vec[1]["u"] - param_Vec[2]["u"];
		param_Vec[point_x - 1]["rho"] = 2 * param_Vec[point_x - 2]["rho"] - param_Vec[point_x - 3]["rho"];
		param_Vec[point_x - 1]["u"] = 2 * param_Vec[point_x - 2]["u"] - param_Vec[point_x - 3]["u"];
		param_Vec[point_x - 1]["T"] = 2 * param_Vec[point_x - 2]["T"] - param_Vec[point_x - 3]["T"];
		cout_array_map(&param_Vec);
	}
}

int main() {
	cout << "      " << "A" << "   " << "rho" << "   " << "u" << "   " << "T" << endl;
	nozzle_init();
	return 0;
}

/*
�Ż�����
1.��oop��װһ���������,����������ڵ���

*/
