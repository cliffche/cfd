#pragma once
#ifndef _SOLVER_H_
#define _SOLVER_H_
//求解器抽象类
class Solver
{
public:
	Solver();
	virtual ~Solver();
	virtual void solve() = 0;//纯虚
private:

};

Solver::Solver()
{
}

Solver::~Solver()
{
}

#endif // !_SOLVER_H_
