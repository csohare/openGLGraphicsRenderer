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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
           -0.5f,  -0.5f, 0.0f, 0.0f, //0
            0.5f,  -0.5f, 1.0f, 0.0f, //1
            0.5f,   0.5f, 1.0f, 1.0f, //2
           -0.5f,   0.5f, 0.0f, 1.0f  //3
        };

        unsigned int indicies[] =
        {
            0, 1, 2,    //triangle 1
            0, 3, 2     //triangle 2
        };


        //GLCall(glEnable(GL_BLEND));
        //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

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

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //CLEAR THE CONTEXT
            renderer.Clear();

            //PROGRAM MUST BE SET BEFORE UNIFORMS ARE DEFINED
            shader.Bind();

            renderer.Draw(va, indBuff, shader);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}