#include"VertexBuffer.h"
#include <glad/glad.h>
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	//generate a buffer for the d.Suliman:
	glGenBuffers(1, &m_RendererID);

	//select the working buffer or something like this:
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

	//charge the buffer with data:
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

}

VertexBuffer::~VertexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind()const
{
	//select the working buffer or something like this:
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind()const
{
	//select the working buffer or something like this:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
