#pragma once
//׼һά���������-�����ٵ������� Maccomack�ƽ���ʽ quasi_one_dimensional_nozzle_flow
#ifndef _QUASI_ONE_DIMENSIONAL_NOZZLE_FLOW_H_ 
#define _QUASI_ONE_DIMENSIONAL_NOZZLE_FLOW_H_ 
#include <armadillo>
class Nozzle_flow
{
public:
	Nozzle_flow();
	~Nozzle_flow();

private:

	double t;//������ʱ��
	double delta_t;//delta t
	//arma::mat(13, 13);

};

Nozzle_flow::Nozzle_flow()
{
}

Nozzle_flow::~Nozzle_flow()
{
}

#endif // !_QUASI_ONE_DIMENSIONAL_NOZZLE_FLOW_H_ 

