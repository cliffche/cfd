#pragma once
#ifndef _TDMA_H_
#define _TDMA_H_
#include <armadillo>
//���ԽǾ����׼�ⷨ(Thomas��)
class Tdma
{

public:
	Tdma();
	~Tdma();
	arma::vec& solver(arma::mat&);
private:
	size_t row;

};

Tdma::Tdma()
{
}

Tdma::~Tdma()
{
}

#endif // !_TDMA_H_
