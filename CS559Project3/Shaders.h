/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#pragma once
#include <GL/glew.h>
#include <map>
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glslprogram.h"

#define SHADER_SOLID 0
#define SHADER_ADS 1
#define SHADER_STAR 2
#define SHADER_TEXTURE 3
#define SHADER_NOISE 4
#define PPO_IDENTITY 5
#define PPO_INVERTED 6

class ShaderFlyweight {
public:
	/* ShaderFlyweight is a Singleton */
	static ShaderFlyweight instance;
	static ShaderFlyweight *inst();
	GLSLProgram *getShader(int name);
	void addShader(int name, GLSLProgram *shader);
	bool initialize();
	void takeDown();

private:
	std::map<int, GLSLProgram *> loadedShaders;
	ShaderFlyweight();
	~ShaderFlyweight();

};

class PostProcessingShader : public GLSLProgram {
public:
	virtual void setup(const glm::mat4 &model);
};
