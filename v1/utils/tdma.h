#pragma once
#ifndef _TDMA_H_
#define _TDMA_H_
#include <armadillo>
//三对角矩阵标准解法(Thomas法)
class Tdma
{
public:
	Tdma(const arma::mat&, arma::vec&);//用矩阵+向量来初始化
	arma::vec& solve() const;//求解矩阵的方法
private:
	size_t row;
	mutable arma::mat mat_A;//三对角矩阵A
	mutable arma::vec vec_b;//列向量b
	mutable arma::vec vec_cache;//缓存行向量
};
#endif // !_TDMA_H_
