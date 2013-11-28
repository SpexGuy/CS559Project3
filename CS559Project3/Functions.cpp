/* See Function.h for documentation */
#include "Function.h"

using namespace glm;


float LinearTimeFunction::evaluate(int time) {
	return m*time + b;
}


vec3 Vec3TimeFunction::evaluate(int time) {
	return vec3(f1->evaluate(time),
				f2->evaluate(time),
				f3->evaluate(time));
}
Vec3TimeFunction::~Vec3TimeFunction() {
	delete f1;
	delete f2;
	delete f3;
}
