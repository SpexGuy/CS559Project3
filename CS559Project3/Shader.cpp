/* This class was based on an example by Perry Kivolowitz.
 */
#include "Shader.h"
#include <assert.h>

using namespace std;
using namespace glm;

#define BAD_GL_VALUE GLuint(-1)

Shader::Shader()
{
	this->vertex_shader_id = BAD_GL_VALUE;
	this->fragment_shader_id = BAD_GL_VALUE;
	this->program_id = BAD_GL_VALUE;
	this->modelview_matrix_handle = BAD_GL_VALUE;
	this->projection_matrix_handle = BAD_GL_VALUE;
	this->normal_matrix_handle = BAD_GL_VALUE;
	this->size_handle = BAD_GL_VALUE;
	this->light_handle = BAD_GL_VALUE;
	this->color_handle = BAD_GL_VALUE;
	this->ambient_handle = BAD_GL_VALUE;
	this->diffuse_handle = BAD_GL_VALUE;
	this->specular_handle = BAD_GL_VALUE;
	this->shiny_handle = BAD_GL_VALUE;

}

/*	This Shader() class implements or assumes a basic set of uniforms will be
	provided to all shaders derived from it. These are listed below. 
	Shader::CommonSetup() can be used by call derived classes to send the 
	common values to the shader. Values unique to the derived class can be
	loaded with the CustomShader() function.
*/

void Shader::commonSetup(const int time, const GLint * size, const GLfloat * projection, const GLfloat * modelview,
						 const GLfloat * mvp, const GLfloat * nm, const GLfloat * light, const GLfloat * color,
						 const GLfloat *ambient, const GLfloat *diffuse, const GLfloat *specular, const float shiny,
						 const int texIndex) const
{
	if (this->time_handle != BAD_GL_VALUE)
		glUniform1d(this->time_handle, time);
	this->GLReturnedError("Shader::commonSetup - after time_handle");
	if (this->size_handle != BAD_GL_VALUE)
		glUniform2iv(this->size_handle, 1, size);
	this->GLReturnedError("Shader::commonSetup - after size_handle");
	if (this->projection_matrix_handle != BAD_GL_VALUE)
		glUniformMatrix4fv(this->projection_matrix_handle, 1, GL_FALSE, projection);
	this->GLReturnedError("Shader::commonSetup - after projection_matrix_handle");
	if (this->modelview_matrix_handle != BAD_GL_VALUE)
		glUniformMatrix4fv(this->modelview_matrix_handle, 1, GL_FALSE, modelview);
	this->GLReturnedError("Shader::commonSetup - after modelview_matrix_handle");
	if (this->mvp_handle != BAD_GL_VALUE)
		glUniformMatrix4fv(this->mvp_handle, 1, GL_FALSE, mvp);
	this->GLReturnedError("Shader::commonSetup - after mvp_handle");
	if (this->normal_matrix_handle != BAD_GL_VALUE)
		glUniformMatrix3fv(this->normal_matrix_handle, 1, GL_FALSE, nm);
	this->GLReturnedError("Shader::commonSetup - after normal_matrix_handle");
	if (this->light_handle != BAD_GL_VALUE)
		glUniform3fv(this->light_handle, 1, light);
	this->GLReturnedError("Shader::commonSetup - after light_handle");
	if (this->color_handle != BAD_GL_VALUE)
		glUniform4fv(this->color_handle, 1, color);
	this->GLReturnedError("Shader::commonSetup - after color_handle");
	if (this->ambient_handle != BAD_GL_VALUE)
		glUniform3fv(this->ambient_handle, 1, ambient);
	this->GLReturnedError("Shader::commonSetup - after ambient_handle");
	if (this->diffuse_handle != BAD_GL_VALUE)
		glUniform3fv(this->diffuse_handle, 1, diffuse);
	this->GLReturnedError("Shader::commonSetup - after diffuse_handle");
	if (this->specular_handle != BAD_GL_VALUE)
		glUniform4fv(this->specular_handle, 1, specular);
	this->GLReturnedError("Shader::commonSetup - after specular_handle");
	if (this->shiny_handle != BAD_GL_VALUE)
		glUniform1f(this->shiny_handle, shiny);
	this->GLReturnedError("Shader::commonSetup - after shiny_handle");
	if (this->texture_handle != BAD_GL_VALUE)
		glUniform1i(this->texture_handle, texIndex);
	this->GLReturnedError("Shader::commonSetup - after texture_handle");

}

void Shader::use() const
{
	assert(this->program_id != BAD_GL_VALUE);
	glUseProgram(this->program_id);
}

/*	The shader initialization code is lifted liberally from the GLSL 4.0 Cookbook.
*/

bool Shader::initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	GLint check_value;

	if (GLReturnedError("Shader::Initialize - on entrance"))
		return false;

	this->vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	this->loadShader(vertex_shader_file, this->vertex_shader_id);
	glCompileShader(this->vertex_shader_id);
	glGetShaderiv(this->vertex_shader_id, GL_COMPILE_STATUS, &check_value);
	if (check_value != GL_TRUE)
	{
		cerr << this->getShaderLog(vertex_shader_id).str();
		cerr << "GLSL compilation failed - vertex shader: " << vertex_shader_file << endl;
		return false;
	}

	if (GLReturnedError("Shader::Initialize - after processing vertex shader"))
		return false;

	this->fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	this->loadShader(fragment_shader_file, this->fragment_shader_id);
	glCompileShader(this->fragment_shader_id);
	glGetShaderiv(this->fragment_shader_id, GL_COMPILE_STATUS, &check_value);
	if (check_value != GL_TRUE)
	{
		cerr << this->getShaderLog(fragment_shader_id).str();
		cerr << "GLSL compilation failed - fragment shader: " << fragment_shader_file << endl;
		return false;
	}

	this->program_id = glCreateProgram();
	glAttachShader(this->program_id, this->vertex_shader_id);
	glAttachShader(this->program_id, this->fragment_shader_id);
	glLinkProgram(program_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	glUseProgram(this->program_id);

	this->modelview_matrix_handle = glGetUniformLocation(this->program_id, (const GLchar *) "modelview_matrix");
	this->projection_matrix_handle = glGetUniformLocation(this->program_id, (const GLchar *) "projection_matrix");
	this->normal_matrix_handle = glGetUniformLocation(this->program_id, (const GLchar *) "normal_matrix");
	this->mvp_handle = glGetUniformLocation(this->program_id, (const GLchar *) "mvp");
	this->size_handle = glGetUniformLocation(this->program_id, (const GLchar *) "size");
	this->time_handle = glGetUniformLocation(this->program_id, (const GLchar *) "time");
	this->light_handle = glGetUniformLocation(this->program_id, (const GLchar *) "light_position");
	this->color_handle = glGetUniformLocation(this->program_id, (const GLchar *) "color");
	this->ambient_handle = glGetUniformLocation(this->program_id, (const GLchar *) "ambientScale");
	this->diffuse_handle = glGetUniformLocation(this->program_id, (const GLchar *) "diffuseScale");
	this->specular_handle = glGetUniformLocation(this->program_id, (const GLchar *) "specularColor");
	this->shiny_handle = glGetUniformLocation(this->program_id, (const GLchar *) "shininess");
	this->texture_handle = glGetUniformLocation(this->program_id, (const GLchar *) "textureIndex");

	glUseProgram(0);

	return !GLReturnedError("Shader::Initialize - on exit");
}

void Shader::customSetup() const
{
}


void Shader::takeDown()
{
	GLint temp;
	GLsizei size;

	if (this->program_id == (GLuint) -1)
		return;

	glGetProgramiv(this->program_id, GL_ATTACHED_SHADERS, &temp);
	if (temp > 0)
	{
		GLuint * shader_list = new GLuint[temp];
		glGetAttachedShaders(this->program_id, temp, &size, shader_list);
		for (GLsizei i = 0; i < size; i++)
		{
			glDetachShader(this->program_id, shader_list[i]);
			// The shaders were marked for deletion
			// immediately after they were created.
		}
		delete [] shader_list;
	}

	glDeleteProgram(this->program_id);
	this->program_id = (GLuint) -1;
}

/*
	This function is adapted from OpenGL 4.0 Shading Language Cookbook by David Wolff.
*/

bool Shader::loadShader(const char * file_name, GLuint shader_id)
{
	assert(file_name != NULL);
	if (GLReturnedError("Shader::LoadShader - on entrance"))
		return false;

	FILE * file_handle = NULL;
	fopen_s(&file_handle, file_name, "rb");
	if (file_handle == NULL)
	{
		cerr <<  "Cannot open shader: " << file_name << endl;
		return false;
	}
	fseek(file_handle, 0, SEEK_END);
	int length = ftell(file_handle);
	fseek(file_handle, 0, SEEK_SET);
	GLubyte * buffer = new GLubyte[length + 1];
	fread(buffer, sizeof(GLubyte), length, file_handle);
	fclose(file_handle);
	buffer[length] = 0;
	glShaderSource(shader_id, 1, (const char **) &buffer, NULL);
	delete [] buffer;

	return !GLReturnedError("Shader::LoadShader - on exit");
}

/*
	This function is adapted from OpenGL 4.0 Shading Language Cookbook by David Wolff.
*/

stringstream Shader::getShaderLog(GLuint shader_id)
{
	stringstream s;
	GLint log_length;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length <= 0)
		s << "No shader log information available." << endl;
	else
	{
		GLchar * buffer = new GLchar[log_length];
		glGetShaderInfoLog(shader_id, log_length, NULL, buffer);
		s << "Shader log:" << endl;
		s << buffer << endl;
		delete [] buffer;
	}
	return s;
}

bool Shader::GLReturnedError(char * s) const
{
	bool return_error = false;
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}

	return return_error;
}

BackgroundShader::BackgroundShader() : super()
{
	this->color_array_handle = BAD_GL_VALUE;
}

bool BackgroundShader::initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	if (!super::initialize(vertex_shader_file, fragment_shader_file))
		return false;

	this->use();
	this->color_array_handle = glGetUniformLocation(this->program_id, (const GLchar *) "color_array");
	glUseProgram(0);
//	assert(this->color_array_handle != BAD_GL_VALUE);
	return true;
}

void BackgroundShader::customSetup(vec4 * color_array)
{
	glUniform4fv(this->color_array_handle, 4, (GLfloat *) color_array);
}





ShaderFlyweight ShaderFlyweight::instance;
ShaderFlyweight *ShaderFlyweight::inst() {
	return &instance;
}

ShaderFlyweight::ShaderFlyweight() :
	loadedShaders() {
}

bool ShaderFlyweight::initialize() {
	loadShader(SHADER_SOLID, "solid_shader.vert", "solid_shader.frag");
	loadShader(SHADER_ADS, "ads_shader.vert", "ads_shader.frag");
	loadShader(SHADER_STAR, "star_shader.vert", "star_shader.frag");
	loadShader(SHADER_TEXTURE, "texture_shader.vert", "texture_shader.frag");

	return true;
}

Shader *ShaderFlyweight::loadShader(int name, char *vertFile, char *fragFile) {
	Shader *s = new Shader();
	bool init = s->initialize(vertFile, fragFile);
	assert(init);
	loadedShaders[name] = s;
	return s;
}

void ShaderFlyweight::addShader(int name, Shader *shader) {
	assert(loadedShaders.find(name) == loadedShaders.end());
	loadedShaders[name] = shader;
}

Shader *ShaderFlyweight::getShader(int name) {
	assert(loadedShaders.find(name) != loadedShaders.end());
	return loadedShaders[name];
}

void ShaderFlyweight::takeDown() {
	for (map<int, Shader *>::iterator
				iter = loadedShaders.begin(),
				end = loadedShaders.end();
			iter != end;
			iter++)
	{
		iter->second->takeDown();
		delete iter->second;
	}
	loadedShaders.clear();
}

ShaderFlyweight::~ShaderFlyweight() {
	assert(loadedShaders.size() == 0);
}
