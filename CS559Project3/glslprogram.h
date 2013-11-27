//Taken from GLSL Cookbook github at https://github.com/daw42/glslcookbook
#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <string>
using std::string;
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

#include <stdexcept>

class GLSLProgramException : public std::runtime_error {
public:
	GLSLProgramException( const string & msg ) :
		std::runtime_error(msg) { }
};

namespace GLSLShader {
    enum GLSLShaderType {
        VERTEX, FRAGMENT, GEOMETRY,
        TESS_CONTROL, TESS_EVALUATION, COMPUTE
    };
};

class GLSLProgram
{
private:
    int  handle;
    bool linked;
    std::map<string, int> uniformLocations;

    int  getUniformLocation(const char * name );
    bool fileExists( const string & fileName );
    string getExtension( const char * fileName );

	// Make these private in order to make the object non-copyable
	GLSLProgram( const GLSLProgram & other ) { }
	GLSLProgram & operator=( const GLSLProgram &other ) { return *this; }

public:
    GLSLProgram();
    virtual ~GLSLProgram();

	void   compileShader( const char *fileName ) throw (GLSLProgramException);
    void   compileShader( const char * fileName, GLSLShader::GLSLShaderType type ) throw (GLSLProgramException);
    void   compileShader( const string & source, GLSLShader::GLSLShaderType type, 
                          const char *fileName = NULL ) throw (GLSLProgramException);
                          
    void   link() throw (GLSLProgramException);
    void   validate() throw(GLSLProgramException);
    void   use() throw (GLSLProgramException);

	//@Precondition - the shader must be in use
	virtual void setup(const glm::mat4 &model) = 0;

    int    getHandle();
    bool   isLinked();

    void   bindAttribLocation( GLuint location, const char * name);
    void   bindFragDataLocation( GLuint location, const char * name );

    void   setUniform( const char *name, float x, float y, float z);
    void   setUniform( const char *name, const vec2 & v);
    void   setUniform( const char *name, const vec3 & v);
    void   setUniform( const char *name, const vec4 & v);
    void   setUniform( const char *name, const mat4 & m);
    void   setUniform( const char *name, const mat3 & m);
    void   setUniform( const char *name, float val );
    void   setUniform( const char *name, int val );
    void   setUniform( const char *name, bool val );
    void   setUniform( const char *name, GLuint val );

	// Extra setUniform functions for setting arrays!
	void   setUniform( const char * name, int size, vec3 * array);
	void   setUniform( const char * name, int size, vec4 * array);
	void   setUniform( const char * name, int size, float * array);


    void   printActiveUniforms();
    void   printActiveUniformBlocks();
    void   printActiveAttribs();
    
    const char * getTypeString( GLenum type );
};

#endif // GLSLPROGRAM_H
