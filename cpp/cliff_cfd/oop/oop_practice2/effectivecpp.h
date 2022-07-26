#pragma once
#ifndef _EFFECTIVECPP_H_
#define _EFFECTIVECPP_H_
#include <iostream>
class Shape
{
public:
	virtual void draw() const = 0;

private:

};

class Rectangle :public Shape
{
public:
	void draw() const;

private:

};

void	Rectangle::draw() const {
	std::cout << "rectangle draw" << std::endl;
}

#endif // _EFFECTIVECPP_H_
