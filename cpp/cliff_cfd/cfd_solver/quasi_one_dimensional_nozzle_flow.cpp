#include <iostream>
#include <math.h>
#include <map>
#include <array>
#include <vector>

using namespace std;
//void nozzle_solver(int point_x) {
//
//	double A;//无量纲宽度
//	double rho;//无量纲密度
//	double T;//无量纲温度	
//	double u;//无量纲速度
//	double pd_rho_t_pre;//偏rho偏t预测值
//	double pd_u_t_pre;//偏u偏t预测值
//	double pd_T_t_pre;//偏T偏t预测值
//	double rho_t_pre;//rho预测量
//	double u_t_pre;//u预测量
//	double T_t_pre;//T预测量
//	double pd_rho_t_cor;//偏rho偏t修正值
//	double pd_u_t_cor;//偏u偏t修正值
//	double pd_T_t_cor;//偏T偏t修正值
//}

//A与x的映射关系
double calculate_A_value(double x) {
	return 1 + 2.2 * (pow((x - 1.5), 2));
}

//计算时间步,取count数等于0.5
double time_step(vector < map<string, double> >* vec, double delta_x, double Courant_value) {
	double temp_value_max = 0;//中间值 根号T+u
	double temp_value;
	vector< map<string, double> > ::iterator it = vec->begin();
	while (it != vec->end()) {
		temp_value = pow((*it)["T"], 0.5) + (*it)["u"];
		temp_value_max = max(temp_value_max, temp_value);
		it++;
	}
	return delta_x * Courant_value / temp_value_max;
}

//cout函数
void cout_array_map(vector<map<string, double>>* vec) {
	vector<map<string, double>> ::iterator it = vec->begin();
	while (it != vec->end()) {
		cout << (*it)["x_value"] << "   " << (*it)["A"] << "   " << (*it)["rho"] << "   " << (*it)["u"] << "   " << (*it)["T"] << endl;
		it++;
	}
}

void nozzle_init() {
	//构建数据结构和初始参数
	const size_t point_x = 31;//网格节点数
	double x_len = 3; //x长度
	double delta_x = 0.1; //x长度
	double delta_x_reciprocal = 1 / delta_x;
	double Courant_value = 0.5;//Courant数
	size_t time_step_total = 20;//总时间步
	double gama_air = 1.4; //空气的γ 1.4	
	map<string, double> point_0 = {
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
	//给计算的初始条件和边界条件
	// 边界rho0 = 1 T0 = 1
	// 初始rho = 1-0.3146x T = 1-0.2314x u = (0.1+1.09x)*(T^1/2)
	//初始化数据结构 vector里装map的数据结构,语义化最好
	vector < map<string, double> > param_Vec(point_x, point_0);
	//计算每一点的A和x_value
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
		//计算delta_t
		cout << t_i << endl;
		double t_step = time_step(&param_Vec, delta_x, Courant_value);
		cout << "t_step= :" << t_step << endl;
		for (size_t j = 0; j < point_x - 1; j++) {//j=30无前差
			//因为是显式,所有的计算参数都是t时刻的,用next表示i+1点		
			double A_this = param_Vec[j]["A"];//A_i_t
			double A_next = param_Vec[j + 1]["A"];//A_i+1_t
			double rho_this = param_Vec[j]["rho"];//rho_i_t
			double rho_next = param_Vec[j + 1]["rho"];//rho_i+1_t
			double T_this = param_Vec[j]["T"];//T_i_t
			double T_next = param_Vec[j + 1]["T"];//T_i+1_t
			double u_this = param_Vec[j]["u"];//u_i_t
			double u_next = param_Vec[j + 1]["u"];//u_i+1_t
			double u_difference_pre = (u_next - u_this) * delta_x_reciprocal;//u前差
			double T_difference_pre = (T_next - T_this) * delta_x_reciprocal;//T前差
			double rho_difference_pre = (rho_next - rho_this) * delta_x_reciprocal;//rho前差
			double ln_A_difference_pre = (log(A_next / A_this)) * delta_x_reciprocal;//ln A前差
			//计算偏rho偏t预测值 rho预测量
			double pd_rho_t_pre = -rho_this * u_difference_pre
				- rho_this * u_this * ln_A_difference_pre
				- u_this * rho_difference_pre;
			double rho_t_pre = rho_this + pd_rho_t_pre * t_step;
			//计算偏u偏t预测值 u预测量
			double pd_u_t_pre = -u_this * u_difference_pre
				- (1 / gama_air) * (T_difference_pre + T_this / rho_this * rho_difference_pre);
			double u_t_pre = u_this + pd_u_t_pre * t_step;
			//计算偏T偏t预测值 T预测量
			double pd_T_t_pre = -u_this * T_difference_pre - (gama_air - 1) * T_this * (u_difference_pre + u_this * ln_A_difference_pre);
			double T_t_pre = T_this + pd_T_t_pre * t_step;
			//存入vector<map>中,去计算修正步
			param_Vec[j]["pd_rho_t_pre"] = pd_rho_t_pre;
			param_Vec[j]["rho_t_pre"] = rho_t_pre;
			param_Vec[j]["pd_u_t_pre"] = pd_u_t_pre;
			param_Vec[j]["u_t_pre"] = u_t_pre;
			param_Vec[j]["pd_T_t_pre"] = pd_T_t_pre;
			param_Vec[j]["T_t_pre"] = T_t_pre;
		}
		// rho和T已知,预测值改回真实值
		param_Vec[0]["T_t_pre"] = 1;
		param_Vec[0]["rho_t_pre"] = 1;
		for (size_t k = 1; k < point_x - 1; k++) {//k=0无后差,k=max后差无意义
			//计算修正步和最终结果命名方式同预测步
			double pd_rho_t_pre = param_Vec[k]["pd_rho_t_pre"];
			double pd_u_t_pre = param_Vec[k]["pd_u_t_pre"];
			double pd_T_t_pre = param_Vec[k]["pd_T_t_pre"];

			double rho_t_pre_this = param_Vec[k]["rho_t_pre"];
			double rho_t_pre_last = param_Vec[k - 1]["rho_t_pre"];
			double rho_t_pre_difference_cor = (rho_t_pre_this - rho_t_pre_last) * delta_x_reciprocal;//rho后差

			double u_t_pre_this = param_Vec[k]["u_t_pre"];
			double u_t_pre_last = param_Vec[k - 1]["u_t_pre"];
			double u_t_pre_difference_cor = (u_t_pre_this - u_t_pre_last) * delta_x_reciprocal;//u后差

			double T_t_pre_this = param_Vec[k]["T_t_pre"];
			double T_t_pre_last = param_Vec[k - 1]["T_t_pre"];
			double T_t_pre_difference_cor = (T_t_pre_this - T_t_pre_last) * delta_x_reciprocal;//T后差

			double A_this = param_Vec[k]["A"];//A_i_t
			double A_last = param_Vec[k - 1]["A"];//A_i+1_t
			double ln_A_difference_pre = (log(A_this / A_last)) * delta_x_reciprocal;//ln A后差
			//计算修正值
			double pd_rho_t_cor = -rho_t_pre_this * u_t_pre_difference_cor
				- rho_t_pre_this * u_t_pre_this * ln_A_difference_pre
				- u_t_pre_this * rho_t_pre_difference_cor;
			double pd_u_t_cor = -u_t_pre_this * u_t_pre_difference_cor
				- (1 / gama_air) * (T_t_pre_difference_cor + (T_t_pre_this / rho_t_pre_this) * rho_t_pre_difference_cor);
			double pd_T_t_cor = -u_t_pre_this * T_t_pre_difference_cor - (gama_air - 1) * T_t_pre_this * (u_t_pre_difference_cor + u_t_pre_this * ln_A_difference_pre);
			//预测与修正的平均值
			double pd_rho_t_average = (pd_rho_t_cor + pd_rho_t_pre) / 2;
			double pd_u_t_average = (pd_u_t_cor + pd_u_t_pre) / 2;
			double pd_T_t_average = (pd_T_t_cor + pd_T_t_pre) / 2;
			//更新rho , u, T
			param_Vec[k]["rho"] += pd_rho_t_average * t_step;
			param_Vec[k]["u"] += pd_u_t_average * t_step;
			param_Vec[k]["T"] += pd_T_t_average * t_step;
		}
		//更新0点和最后一点的值 rho和T已知
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
优化方案
1.用oop封装一个输出方法,输出美观易于调试

*/
