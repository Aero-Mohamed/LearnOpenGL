#pragma once
#include "Renderer.h"
#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned int normalized;
	static unsigned int getTypeSize(unsigned int type) {
		switch (type) {
			case GL_FLOAT: return sizeof(float);
			case GL_UNSIGNED_INT: return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
		}
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout(): m_Stride(0){ };

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(true);
	}

	template<>
	void Push<float>(unsigned int count) 
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_Stride += VertexBufferElement::getTypeSize(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::getTypeSize(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
		m_Stride += VertexBufferElement::getTypeSize(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> getElements() const { return m_Elements; }
	inline unsigned int getStride() const { return m_Stride; }

};

