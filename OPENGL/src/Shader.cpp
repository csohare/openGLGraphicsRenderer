#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Renderer.h"



Shader::Shader(const std::string& filePath)
	:m_FilePath(filePath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(m_FilePath);
    m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
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


unsigned int Shader::CompileShader(unsigned int type, const std::string& source) 
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

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)

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

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID))
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& UniformName, float v0, float v1, float v2, float v3)
{
    SetUniformLocation(UniformName);
    GLCall(glUniform4f(m_UniformCache[UniformName], v0, v1, v2, v3));
}

void Shader::SetUniformLocation(const std::string& UniformName)
{
    if (m_UniformCache.find(UniformName) == m_UniformCache.end())
    {
		GLCall(int location = glGetUniformLocation(m_RendererID, UniformName.c_str()));
		if(location == -1) 
		{
			std::cout << "WARNING: Uniform '" << UniformName << "' Doesn't Exist!" << std::endl;
		}
        m_UniformCache[UniformName] = location;
    }
}
