#pragma once
#include <glm/glm.hpp>

class VertexPN {
public:
	glm::vec3 position;
	glm::vec3 normal;
};

class VertexPC {
public:
	glm::vec3 position;
	glm::vec4 color;
};

class VertexPCN {
public:
	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 normal;
};

class VertexPNT {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
};