#pragma once

#include "Test.h"

#include <memory>

#include "Buffer.h"
#include "VertexArray.h"
#include "BufferLayout.h"
#include "Shader.h"
#include "Texture.h"



namespace test {
	
	class TestTexture : public Test
	{
	public:

		TestTexture();
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;  

	private:

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Buffer> m_INDEXBUFFER;
		std::unique_ptr<Shader> m_SHADER;
		std::unique_ptr<Texture> m_TEXTURE;
		std::unique_ptr<Buffer> m_VERTEXBUFFER;

		glm::mat4 m_PROJ, m_VIEW;

		glm::vec3 m_TRANSLATIONA, m_TRANSLATIONB;


	};
}
