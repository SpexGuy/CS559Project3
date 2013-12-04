#pragma once
#include <glm/glm.hpp>

class SplinePoint1 {
public:
	SplinePoint1() {}
	SplinePoint1(float distance, float position) :
		distance(distance), position(position) {}

	float distance;
	float position;

	float getAfterPoint() const;
	float getBeforePoint() const;
};

class SplinePoint3 {
public:
	SplinePoint3() {}
	SplinePoint3(float distance, glm::vec3 position,
				float angle, float axisAngle) :
			distance(distance), position(position),
			angle(angle), axisAngle(axisAngle) {}

	float distance;
	glm::vec3 position;
	float angle;
	float axisAngle;

	glm::vec3 getAfterPoint() const;
	glm::vec3 getBeforePoint() const;
};

glm::vec3 bezier(const SplinePoint3 &before, const SplinePoint3 &after, float pos);

float bezier(const SplinePoint1 &before, const SplinePoint1 &after, float pos);


