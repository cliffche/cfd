#pragma once
#ifndef _SOLVER_H_
#define _SOLVER_H_
//�����������
class Solver
{
public:
	Solver();
	virtual ~Solver();
	virtual void solve() = 0;//����
private:

};

Solver::Solver()
{
}

Solver::~Solver()
{
}

#endif // !_SOLVER_H_
