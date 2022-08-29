#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "Buffer.h"
#include "VertexArray.h"



struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};




static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

	std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;



    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else


        {
            ss[(int)type] << line << "\n";

        }
    }

    return { ss[0].str(), ss[1].str() };


}


static unsigned int CompileShader(unsigned int type, const std::string& source) 
{

    //Creating shader object and non zero value to reference it
    // Used to maintain source code strings that define a shader.
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    // id is the value used to reference the shader object you want to overwrite.
    // Any source code previously on the shader object is replaced.
    GLCall(glShaderSource(id, 1, &src, nullptr));

    //Compile the specified shader referenced by ID
    GLCall(glCompileShader(id));

    //TODO: Error Handling

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "FAILED TO COMPILE SHADER" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }



    return id;

}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)

{
    //ALL OPERATIONS PERFORMED ON A SHADER OBJECT CAN BE DONE BEFORE/AFTER YOU ATTATCH IF TO YOUR PROGRAM


    /*Creates empty program object.Creates non - zero value where it can be referred to
    // Shader object can be attatched to program objects. Shader objects can be detatched from a program objects 
    // when needed. */
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //program is the specific program object you want to attach your shader objects to. 
    //This helps you specify the list of compiled shaders you want to link together.
    //You are allowed to attatch multiple of the same type of shader to the same program
    //One shader object can also be attatched to different programs at the same time if needed

    //if a shader object is deleted while it is still attached. It will be flagged for deletion. 
    //Deletion can be done by calling glDetatchShader to detatch it from all attached programs.
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    

    //Links the specified program.
    //GL_VERTEX_SHADER are used to create an exe that will run on the programmable vertex processor
    //GL_GEOMETRY_SHADER are used to create an exe that will run on the programmable geometry processor
    //GL_FRAMENT_SHADER are used to create an exe that will run on the programmable fragment processor
    GLCall(glLinkProgram(program));


    //Checks whether the executables contained inside of program can run given the current OpenGL state
    GLCall(glValidateProgram(program));
 
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;




}

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
           -0.5f,  -0.5f, //0
            0.5f,  -0.5f, //1
            0.5f,   0.5f, //2
           -0.5f,   0.5f, //3
        };

        unsigned int indicies[] =
        {
            0, 1, 2,    //triangle 1
            0, 3, 2     //triangle 2
        };


        //CREATE NEW VERTEX ARRAY OBJECT. BINDS IT AUTOMATICALLY
        VertexArray va;

        //CREATING NEW BUFFER FOR VERTEX POSITIONS
        Buffer arrBuffer(positions, 8, sizeof(float), GL_ARRAY_BUFFER);
        //CREATING NEW BUFFER FOR THE INDICIES
        Buffer indBuff(indicies, 6, sizeof(unsigned int), GL_ELEMENT_ARRAY_BUFFER);

        //BufferLayout layout;
        BufferLayout layout;
        layout.Push<float>(2);
        va.addBuffer(arrBuffer, layout);

        ShaderProgramSource source = ParseShader("res\\shaders\\Basic.shader");

        unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);

        GLCall(glUseProgram(shader));

        //Must have shader bound first before uniforms


        //Returns location of your specific uniform variable which can be referenced by its return value
        //Must pass in the current program in use as uniforms are bound to them.
        GLCall(int u_Local = glGetUniformLocation(shader, "u_Color"));
        ASSERT(u_Local != -1);
        //Using the location from glGetUniformLocation, you can modify what it stores here
        GLCall(glUniform4f(u_Local, 0.0f, 0.0f, 0.0f, 1.0f));


        //UNBINDING EVERYTHING
        GLCall(glUseProgram(0));
        va.Unbind();
        arrBuffer.Unbind();
        indBuff.Unbind();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(shader));


            //PROGRAM MUST BE SET BEFORE UNIFORMS ARE DEFINED
            GLCall(glUniform4f(u_Local, 0.5f, 0.5f, 0.5f, 1.0f));


            //BINDING VA REMEMBERS VBUFF AND IBUFF SO NO NEED TO BIND THOSE TWO
            va.Bind();


            //Specify what type of primitive to render,
            //The starting index in the enabled array and the number of indicies to render
            //Uniforms are set for each draw while attributes are set on each vertex
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));





            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        GLCall(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}