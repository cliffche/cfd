#include "couette.h"
#include <list>
using namespace std;
//带参数的构造函数:网格数量,总时间步,Δt,雷诺数
Couette::Couette(size_t mesh_qty, size_t total_time_step, double delta_t, double Reynold_number) :
	mesh_qty(mesh_qty), total_time_step(total_time_step), delta_t(delta_t), Reynold_number(Reynold_number)
{
	delta_y = 1.0 / (mesh_qty + 1);
	construct_parameter_E = delta_t / (Reynold_number * pow(delta_y, 2));
	mat_A = arma::mat(mesh_qty, 3);
	vec_b = arma::vec(mesh_qty);
}
//调用tmda类计算矩阵
void Couette::compute() const {
	Tdma tmda(mat_A, vec_b);
	vec_b = tmda.solve();
}
void Couette::update_vecb() {
	double b_last = 0;
	double b_this = 0;
	double b_neg2 = vec_b.at(mesh_qty - 2);//缓存u-负2
	vec_b.at(0) = (1 - construct_parameter_E) * vec_b.at(0) + 0.5 * construct_parameter_E * vec_b.at(1);
	for (size_t i = 0; i < mesh_qty; i++) {
		b_this = vec_b.at(i);//缓存上一点的值
		if (i != 0 && i != (mesh_qty - 1)) {
			vec_b.at(i) = (1 - construct_parameter_E) * vec_b.at(i) + 0.5 * construct_parameter_E * (vec_b.at(i + 1) + b_last);
		}
		b_last = b_this;
	}
	vec_b.at(mesh_qty - 1) = (1 - construct_parameter_E) * vec_b.at(mesh_qty - 1) + 0.5 * construct_parameter_E * (b_neg2 + 1) + 0.5 * construct_parameter_E;
}

void Couette::matrix_init() {
	double param_a = -0.5 * construct_parameter_E;
	double param_b = 1 + construct_parameter_E;
	double param_c = param_a;
	for (int i = 0; i < mesh_qty; i++) {
		mat_A.at(i, 0) = param_a;
		mat_A.at(i, 1) = param_b;
		mat_A.at(i, 2) = param_c;
		vec_b.at(i) = 0;
	}
	mat_A.at(0, 0) = 0;
	mat_A.at(mesh_qty - 1, 2) = 0;
	vec_b.at(mesh_qty - 1) = construct_parameter_E;
}

void Couette::solve() {
	matrix_init();
	for (size_t i = 0; i < total_time_step; i++)
	{
		update_vecb();
		compute();
		cout << "time step " << i << " result:u" << endl;
		cout << vec_b << endl;
	}
}