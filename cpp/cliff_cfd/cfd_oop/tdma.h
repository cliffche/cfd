#pragma once
#ifndef _TDMA_H_
#define _TDMA_H_
#include <armadillo>
//���ԽǾ����׼�ⷨ(Thomas��)
class Tdma
{
public:
	Tdma(const arma::mat&, arma::vec&);//�þ���+��������ʼ��
	arma::vec& solve() const;//������ķ���
private:
	size_t row;
	mutable arma::mat mat_A;//���ԽǾ���A
	mutable arma::vec vec_b;//������b
	mutable arma::vec vec_cache;//����������
};
#endif // !_TDMA_H_
