#pragma once
#include <GL\glew.h>
#include <vector>

#include "Renderer.h"



struct BufferElement
{
	unsigned int type;
	int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			 return 4;
			case GL_UNSIGNED_INT:	 return 4;
			case GL_UNSIGNED_BYTE: 	 return 1;
		}

		ASSERT(false);
		return 0;
	}
};


class BufferLayout
{
private:
	std::vector<BufferElement> m_Elements;
	int m_Stride;

public:

	BufferLayout()
		: m_Stride(0) {}
	~BufferLayout() {}


	template<typename T>
	void Push(int count)
	{
		static_assert(true);
	}

	template<>
	void Push<float>(int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE});
		m_Stride += BufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int >(int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE});
		m_Stride += BufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>( int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE});
		m_Stride += BufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	inline const std::vector<BufferElement>& getElements()  const { return m_Elements; }
	inline unsigned int getStride() const { return m_Stride; }

};
