#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "BufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(2);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "FAILED glewINIT" << std::endl;
    }
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
        VertexArray va;


        //CREATING NEW BUFFER FOR VERTEX POSITIONS
        Buffer arrBuffer(positions, 8 * 2, sizeof(float), GL_ARRAY_BUFFER);
        //CREATING NEW BUFFER FOR THE INDICIES
        Buffer indBuff(indicies, 6, sizeof(unsigned int), GL_ELEMENT_ARRAY_BUFFER);

        //BufferLayout layout;
        BufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.addBuffer(arrBuffer, layout);

        //CREATING PROJECTION MATRIX
        glm::mat4 projMatrix = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        //Shader Stuff
        Shader shader("res\\shaders\\Basic.shader");
        shader.Bind();


        Texture texture("res\\Textures\\TestTexture2.png");
        texture.Bind();

        ////Telling shader which slot you bound the texture to. In this case it is slot 0.
        shader.SetUniform1i("u_Texture", 0);


        //UNBINDING EVERYTHING
        shader.Unbind();
        va.Unbind();
        arrBuffer.Unbind();
        indBuff.Unbind();

        Renderer renderer;

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //CLEAR THE CONTEXT
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            {
                //SETTING MVP FOR FIRST RENDER. SHADER MUST BE BOUND FIRST TO SET UNIFORMS
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 MVP = projMatrix * viewMatrix * modelMatrix;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", MVP); 

				renderer.Draw(va, indBuff, shader);
            }

            {
                //SETTING MVP FOR FIRST RENDER. SHADER MUST BE BOUND FIRST TO SET UNIFORMS
				glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 MVP = projMatrix * viewMatrix * modelMatrix;
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", MVP); 

				renderer.Draw(va, indBuff, shader);
            }

            {
                ImGui::SliderFloat3("Translate A", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("Translate B", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            
			ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */

            glfwPollEvents();

        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}