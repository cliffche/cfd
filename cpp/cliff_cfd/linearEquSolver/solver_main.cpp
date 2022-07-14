#include <iostream>
#include <armadillo>
#include <typeinfo>
#include "jacobi.h"
using namespace std;
using namespace arma;


int main() {
	mat A(5, 5, fill::randu);
	//cout << typeid(&A).name() << endl;
	solver(&A);
	return 0;
}