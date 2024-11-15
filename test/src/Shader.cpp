#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "GLErrorManager.h"


Shader::Shader(const std::string& filepath): m_FilePath(filepath), m_RendererID(0)
{
	//init shaders//
	//------------------------------//
	//read the shader code from the basic.shader file:
	ShaderProgramSource source = ParseShader(filepath);

	//create the shader:
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	//------------------------------//
	//end of init//
}

Shader::~Shader()
{
	GlCall(glDeleteProgram(m_RendererID));
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	//glCreateProgram is a function in OpenGL that creates an empty shader program object, which will eventually hold shaders.
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//attach our shaders to the program 
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	//glLinkProgram combines all the attached shaders in the specified program into a single, executable shader program.
	//This function processes the individual shaders (e.g., vertex, fragment) that were previously attached to program.
	//It checks that they are compatible (e.g., outputs of one shader match inputs of the next) and merges them.
	//After linking, the program can be used in the OpenGL pipeline.
	glLinkProgram(program);

	//glValidateProgram checks if the specified program can execute in the current OpenGL state.
	//This function tests whether the program can run given the current OpenGL context 
	//and settings. It is particularly useful in debugging to ensure the program can 
	//function properly with the current framebuffer, attributes, and other OpenGL states.
	glValidateProgram(program);

	//delete shaders:
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {

	//create a shader and get the unique id of it:
	GlCall(unsigned int id = glCreateShader(type););

	//a pointer to the first character in the string:
	const char* src = source.c_str();

	//add the source code to the shader:
	glShaderSource(id, 1, &src, nullptr);

	//compile our shader:
	glCompileShader(id);

	//error handling:
	int result;

	//retrieve the compilation status:
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {

		int length;

		//Gets the length of the shader’s information log:
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));

		//Retrieves the actual error log for the shader:
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to cmpile " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	//return the id that will we use to access the shader:
	return id;
}


//read shaders from file:
ShaderProgramSource Shader::ParseShader(const std::string& filepath) {

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1,
	};

	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };
}


void Shader::Bind() const
{
	GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GlCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	//assign the color values to the uniform inside gpu memory:
	GlCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
	//assign the color values to the uniform inside gpu memory:
	GlCall(glUniform1f(GetUniformLocation(name), v0));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformlocationCache.find(name)!=m_UniformlocationCache.end()) {
		return m_UniformlocationCache[name];
	}

	//get the id of the uniform form the gpu to use it:
	GlCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
		m_UniformlocationCache[name] = location;
	return location;
}
