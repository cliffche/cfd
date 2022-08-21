//һά����ѹ��couette�� incompre_couette_one_dimensional
#include <iostream>
#include "incompre_couette_one_dimensional.h"
#include <armadillo>
#include <list>
using namespace arma;
using namespace std;
//���ԽǾ��������˹�㷨
dmat* solver_tdma_icod(dmat* a) {
	int len = a->n_rows;//����
	vec vec_cache(len);//������
	//1.˳����Ԫһ��
	//��������
	a->at(0, 0) = 0;
	a->at(0, 2) = a->at(0, 2) / a->at(0, 1);
	a->at(0, 3) = a->at(0, 3) / a->at(0, 1);
	a->at(0, 1) = 1;
	//����������
	for (int i = 1; i < len; i++) {
		vec_cache = { a->at(i,0),a->at(i,1), a->at(i,2), a->at(i,3) };//����a1 b1 c1 d1
		//cout << *a << endl;
		a->at(i, 0) = 0;//a2` = 0
		//b2` = b1*b2 - a2*c1 ʵ�ʵ�������b2`���뻯��1,��Ϊ����ľ���ᵼ�º��������� a->at(i, 1) = (a->at(i - 1, 1)) * vec_cache.at(1) - vec_cache.at(0) * (a->at(i - 1, 2));
		double b_temp = (a->at(i - 1, 1)) * vec_cache.at(1) - vec_cache.at(0) * (a->at(i - 1, 2));
		a->at(i, 1) = 1;
		a->at(i, 2) = vec_cache.at(2) * (a->at(i - 1, 1)) / b_temp;//c2` = b1 * c2 /(b1*b2 - a2*c1)
		a->at(i, 3) = (a->at(i - 1, 1) * vec_cache.at(3) - a->at(i - 1, 3) * vec_cache.at(0)) / b_temp; // d2` = (b1*d2 - a2*d1) /(b1*b2 - a2*c1)
	}
	//2.������Ԫһ��
	for (int j = len - 2; j >= 0; j--) {
		//tdma_soluction.at(j) = ;//d3`` = d3` - d4``*c3
		a->at(j, 3) = a->at(j, 3) - a->at(j + 1, 3) * a->at(j, 2);
		a->at(j, 2) = 0;
	}
	cout << *a << endl;
	return a;
}

//void icod_solver(dmat* m_mat) {
//	//cout << *m_mat << endl;
//	dmat* b = solver_tdma_icod(*&m_mat);
//	cout.precision(13);
//	cout.setf(ios::fixed);
//	b->raw_print(cout, "b");
//	//cout << *b << endl;
//}

//���������ٵļ��������ͳ�ʼֵ
void icod_init() {
	const size_t total_mesh_y = 19;//y��������(�߽粻������)
	const size_t total_time_step = 12;//���㲽��
	//�������E e = delta_t/(Re * (total_mesh_y **2))
	const double construct_parameter_E = 1.25;
	//����Ĳ���
	double param_a = -0.5 * construct_parameter_E;//
	double param_b = 1 + construct_parameter_E;
	double param_c = param_a;
	dmat mat(total_mesh_y, 4);//�� n*4 ����ʽ�洢���ԽǾ����A��b
	//��ʼ������,���һ��Ϊ1,����Ϊ0
	for (int i = 0; i < total_time_step; i++) {
		cout << i << endl;
		if (i == 0) {//��һ�ε���,Ҫ��d�и���ʼֵ
			//ͨ�����������ֵ
			list<double> params_list = { param_a, param_b, param_c,0 };
			list<double>::iterator  param_it = params_list.begin();
			dmat::row_iterator row_it = mat.begin_row(0);
			while (row_it != mat.end_row(total_mesh_y - 1))
			{
				*row_it = *param_it;
				param_it++;
				if (param_it == params_list.end()) {
					param_it = params_list.begin();
				}
				row_it++;
			}
			//�޸ĵ�һ�к����һ��
			mat.at(0, 0) = 0;
			mat.at(total_mesh_y - 1, 2) = 0;
			mat.at(total_mesh_y - 1, 3) = construct_parameter_E * 0.5 - param_a;
		}
		else {//�����ε���,d�и���ǰһ�μ���Ľ������
			/*double* temp_last = new double(0);
			double* temp_this = new double(0);*/
			double temp_last = 0;
			double temp_this = 0;
			double point_three_first = mat.at(1, 3);//����u3
			double point_three_last = mat.at(total_mesh_y - 2, 3);//����u19
			for (int i = 0; i < total_mesh_y; i++) {
				mat.at(i, 0) = param_a;
				mat.at(i, 1) = param_b;
				mat.at(i, 2) = param_c;
				temp_this = mat.at(i, 3);//������һ���ֵ
				if (i != 0 && i != (total_mesh_y - 1)) {
					mat.at(i, 3) = (1 - construct_parameter_E) * temp_this + 0.5 * construct_parameter_E * (mat.at(i + 1, 3) + temp_last);
				}
				temp_last = temp_this;
			}
			//������β
			//cout << "point_three_d" << point_three_first << endl;
			mat.at(total_mesh_y - 1, 3) = (1 - construct_parameter_E) * mat.at(total_mesh_y - 1, 3) + 0.5 * construct_parameter_E * (point_three_last + 1) - param_a;
			mat.at(0, 3) = (1 - construct_parameter_E) * mat.at(0, 3) + 0.5 * construct_parameter_E * point_three_first;
			mat.at(0, 0) = 0;
			mat.at(total_mesh_y - 1, 2) = 0;
		}
		cout << "mat last" << mat.at(total_mesh_y - 1, 3) << endl;
		cout << "mat before cal" << endl;
		//cout.precision(13);
		//cout.setf(ios::fixed);
		//mat.raw_print(cout, "mat before cal");
		cout << mat << endl;
		dmat* b = solver_tdma_icod(&mat);
		cout.precision(13);
		cout.setf(ios::fixed);
		b->raw_print(cout, "mat after cal");
	}
}

/*
�Ż���Ŀ:
2.�ڴ���ٶȵ��Ż�
3.��װ
�ĵ�:
1.�ų���������ȷ����һ���Ǿ�����������û�������,��С�Ų鷶Χ,���籾���������Է�����Ľⷨ���������,���ֻ��Ҫ�Ų�߽�����
2.�ر�Ҫע��߽������Ĵ���
3.������,������Ҳ��Ҫ�ó������������,Ӱ��δ���ķ�װ
*/