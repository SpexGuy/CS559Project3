#include "Spline.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace glm;



float SplinePoint1::getAfterPoint() const {
	return position + distance;
}

float SplinePoint1::getBeforePoint() const {
	return position - distance;
}



vec3 SplinePoint3::getAfterPoint() const {
	return position +
		vec3(distance * sin(angle * M_PI / 180) * sin(axisAngle * M_PI / 180),
			 distance * cos(axisAngle * M_PI / 180),
			 distance * cos(angle * M_PI / 180) * sin(axisAngle * M_PI / 180));
}

vec3 SplinePoint3::getBeforePoint() const {
	return position -
		vec3(distance * sin(angle * M_PI / 180) * sin(axisAngle * M_PI / 180),
			 distance * cos(axisAngle * M_PI / 180),
			 distance * cos(angle * M_PI / 180) * sin(axisAngle * M_PI / 180));
}



vec3 bezier(const SplinePoint3 &before,
			const SplinePoint3 &after,
			float t)
{
	vec3 p000 = before.position;
	vec3 p001 = before.getAfterPoint();
	vec3 p002 = after.getBeforePoint();
	vec3 p003 = after.position;
	vec3 p00 = mix(p000, p001, t);
	vec3 p01 = mix(p001, p002, t);
	vec3 p02 = mix(p002, p003, t);
	vec3 p0 = mix(p00, p01, t);
	vec3 p1 = mix(p01, p02, t);
	return mix(p0, p1, t);
}

float bezier(const SplinePoint1 &before,
			 const SplinePoint1 &after,
			 float t)
{
	float p000 = before.position;
	float p001 = before.getAfterPoint();
	float p002 = after.getBeforePoint();
	float p003 = after.position;
	float p00 = mix(p000, p001, t);
	float p01 = mix(p001, p002, t);
	float p02 = mix(p002, p003, t);
	float p0 = mix(p00, p01, t);
	float p1 = mix(p01, p02, t);
	return mix(p0, p1, t);
}
