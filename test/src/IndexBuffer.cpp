#include"IndexBuffer.h"
#include <glad/glad.h>
#include "GLErrorManager.h";

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:m_Count(count)
{
	//generate a buffer for the d.Suliman:
	glGenBuffers(1, &m_RendererID);

	//select the working buffer or something like this:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

	//charge the buffer with data:
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int ), data, GL_STATIC_DRAW);

}

IndexBuffer::~IndexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind()const
{
	//select the working buffer or something like this:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind()const
{
	//select the working buffer or something like this:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
