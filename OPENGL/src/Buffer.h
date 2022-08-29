#pragma once

class Buffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_BufferTarget;
	unsigned int m_Count;

public:

	Buffer(const void* data, unsigned int count, unsigned int typeSize, unsigned int target);
	~Buffer();

	void Bind() const;
	void Unbind() const;
	
	inline unsigned int getCount() const;






};
