#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GlCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{

	Bind();
	//bind the vertex buffer (select it to enable operation on it):
	vb.Bind();

	//get the vector that have the values for glVertexAttribPointer:
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++) {

		//get the i-th attribute:
		const auto& element = elements[i];

		//enable the i-th attribute:
		GlCall(glEnableVertexAttribArray(i));

		//determine the layout of the buffer that the gpu will read data from:
		//when we sit the layout the the vertex array object will be linked to the current bound array buffer
		GlCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type); 

	}

}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GlCall(glBindVertexArray(0));
}
