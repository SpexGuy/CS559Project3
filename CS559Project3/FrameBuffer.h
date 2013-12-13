#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class FrameBufferObject : public Texture {
private:
	FrameBufferObject();

protected:
	int number_of_color_attachments;
	int color_buffer_index;
	bool allocateDepthBuffer;
	GLuint framebuffer_handle;
	GLuint depth_buffer_handle;
	GLuint * texture_handles;

public:
	FrameBufferObject(glm::ivec2 size, int number_of_color_attachments = 1, bool allocate_depth_buffer = true) :
		size(size),
		number_of_color_attachments(number_of_color_attachments),
		allocateDepthBuffer(allocate_depth_buffer),
		color_buffer_index(0),
		framebuffer_handle(-1),
		depth_buffer_handle(-1)
	{}

	virtual inline void useBuffer(int color_buffer_index) {
		assert(color_buffer_index <= this->number_of_color_attachments);
		this->color_buffer_index = color_buffer_index;
	}
	virtual inline void resetBufferIndex() {
		useBuffer(0);
	}
	virtual inline void incrementBuffer() {
		useBuffer(color_buffer_index + 1);
	}

	virtual void bindTexture();
	virtual void bindDraw();
	virtual void unbindDraw();
	virtual void resize(int x, int y);
	virtual bool initialize();
	virtual void takeDown();
	virtual ~FrameBufferObject();

	glm::ivec2 size;
};

const char *FramebufferCompletenessError(GLint error_code);
