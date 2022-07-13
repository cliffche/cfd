#include "Shape.h"

class Shape {
protected:
	float width, height;
public:
	void set_data(float a,float b){
		width = a;
		height = b;
}
};
class Square :public Shape {
public:
	float area() {
		return width * height;
	}
};