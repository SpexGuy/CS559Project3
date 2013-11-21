#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class FrameBufferObject : public Texture {
private:
	FrameBufferObject();
public:
	FrameBufferObject(glm::ivec2 size, int number_of_color_attachments = 1, bool allocate_depth_buffer = true) :
		size(size),
		number_of_color_attachments(number_of_color_attachments),
		allocateDepthBuffer(allocate_depth_buffer),
		color_buffer_index(0),
		framebuffer_handle(-1),
		depth_buffer_handle(-1),
		texture_handles(new GLuint[number_of_color_attachments])
	{
		for (int i = 0; i < number_of_color_attachments; i++)
			this->texture_handles[i] = GLuint(-1);
	}

	virtual inline void useBuffer(int color_buffer_index) {
		assert(color_buffer_index <= this->number_of_color_attachments);
		this->color_buffer_index = color_buffer_index;
	}
	virtual void bind();
	virtual void unbind();
	virtual bool initialize();
	virtual void takeDown();
	virtual ~FrameBufferObject();

	glm::ivec2 size;
	int number_of_color_attachments;
	int color_buffer_index;
	bool allocateDepthBuffer;
	GLuint framebuffer_handle;
	GLuint depth_buffer_handle;
	GLuint * texture_handles;
};

const char *FramebufferCompletenessError(GLint error_code);
