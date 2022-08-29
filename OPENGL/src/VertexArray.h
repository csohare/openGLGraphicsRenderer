#pragma once

#include "Buffer.h"
#include "BufferLayout.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void addBuffer(const Buffer& b, const BufferLayout& layout);


};
