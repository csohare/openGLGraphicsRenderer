#include "TestTexture.h" 
#include "imgui\imgui.h"
#include "Renderer.h"


#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"



namespace test {

    TestTexture::TestTexture()
        :m_TRANSLATIONA(200, 200, 0), m_TRANSLATIONB(400, 200, 0),
        m_PROJ(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_VIEW(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{

        float positions[] = {
            -50.0f, -50.0f , 0.0f, 0.0f, //0
            50.0f,  -50.0f , 1.0f, 0.0f, //1
            50.0f,   50.0f, 1.0f, 1.0f, //2
            -50.0f,  50.0f, 0.0f, 1.0f  //3
        };

        unsigned int indicies[] =
        {
            0, 1, 2,    //triangle 1
            0, 3, 2     //triangle 2
        };


        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        //CREATE NEW VERTEX ARRAY OBJECT. BINDS IT AUTOMATICALLY
        m_VAO = std::make_unique<VertexArray>();

        //CREATING NEW BUFFER FOR VERTEX POSITIONS
        m_VERTEXBUFFER = std::make_unique<Buffer>(positions, 8 * 2, sizeof(float), GL_ARRAY_BUFFER);

        //CREATING NEW BUFFER FOR THE INDICIES
        m_INDEXBUFFER = std::make_unique<Buffer>(indicies, 6, sizeof(unsigned int), GL_ELEMENT_ARRAY_BUFFER);

        //BufferLayout layout;
        BufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        //SET THE VERTEX ARRAY ATTRIBUTES FOR VERTEX BUFFER AND BIND IT TO CURRENT VAO
        m_VAO->addBuffer(*m_VERTEXBUFFER, layout);


        //Shader Stuff
        m_SHADER = std::make_unique<Shader>("res\\shaders\\Basic.shader");
        m_SHADER->Bind();
        m_TEXTURE = std::make_unique<Texture>("res\\Textures\\TestTexture2.png");
        m_TEXTURE->Bind();

        ////Telling shader which slot you bound the texture to. In this case it is slot 0.
        m_SHADER->SetUniform1i("u_Texture", 0);
	}

	TestTexture::~TestTexture()
	{
	}

	void TestTexture::OnUpdate(float deltaTime)
	{
	}

	void TestTexture::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        {
            //SETTING MVP FOR FIRST RENDER. SHADER MUST BE BOUND FIRST TO SET UNIFORMS
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_TRANSLATIONA);
            glm::mat4 MVP = m_PROJ * m_VIEW * modelMatrix;
            m_SHADER->Bind();
            m_SHADER->SetUniformMat4f("u_MVP", MVP);

            renderer.Draw(*m_VAO, *m_INDEXBUFFER, *m_SHADER);
        }
        {
            //SETTING MVP FOR FIRST RENDER. SHADER MUST BE BOUND FIRST TO SET UNIFORMS
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_TRANSLATIONB);
            glm::mat4 MVP = m_PROJ * m_VIEW * modelMatrix;
            m_SHADER->Bind();
            m_SHADER->SetUniformMat4f("u_MVP", MVP);

            renderer.Draw(*m_VAO, *m_INDEXBUFFER, *m_SHADER);
        }


	}

	void TestTexture::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translate A", &m_TRANSLATIONA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translate B", &m_TRANSLATIONB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}
