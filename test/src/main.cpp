//a buffer is a data we store in the memory
//a gl buffer is a data we store in vram of gpu

//a shader is a program that run in the gpu
//a vertix shader tell the gpu where the vertix should be in the screen and will be called for each vertix
//a fragment shader will draw each pixel that should be drawn and decide the color of it and will be called for each pixel

//a vertix attribute pointer is used to tell the gpu what each part of the buffer represent weither it is a position or texture or float and so on...

//a vertix contain way more data than position like color and so on...



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();

//error handling for opengl errors that can't be seen by the compiler:
#define GlCall(x) GlClearError();\
	x;\
	ASSERT(GlLogCall(#x,__FILE__,__LINE__))

//clear all previous errors:
static void GlClearError() {
	while (glGetError() != GL_NO_ERROR);
}

//log the error message to the consol 
static bool GlLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGl Error code]: (" << error << ") \n" << "[the function that cause the error]: " << function << "\n[The path of the file that cause the error]: " << file << "\n[The line]: " << line << std::endl;
		return false;
	}
	return true;
}


struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

// دالة تعالج تغيير حجم النافذة
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// دالة لمعالجة إدخال المستخدم (إغلاق النافذة عند الضغط على مفتاح Escape)
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//read shaders from file:
static ShaderProgramSource ParseShader(const std::string& filepath) {

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
	return { ss[0].str(),ss[1].str()};
}

static unsigned int complieShader(unsigned int type, const std::string& source) {

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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	//glCreateProgram is a function in OpenGL that creates an empty shader program object, which will eventually hold shaders.
	unsigned int program = glCreateProgram();
	unsigned int vs = complieShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = complieShader(GL_FRAGMENT_SHADER, fragmentShader);

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

int main() {
	// تهيئة مكتبة GLFW وإنشاء نافذة
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//sycronyize with v-sync:
	glfwSwapInterval(1);
	// 
	// تحميل دوال OpenGL باستخدام GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//vertex array object:
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	float position[] = {
		-0.5f , -0.5f, // 0
		 0.5f , -0.5f, // 1
		 0.5f ,  0.5f, // 2
		-0.5f ,  0.5f, // 3
	};

	unsigned int indices[] = {
		0,1,2,
		2,3,0,
	};

	unsigned int buffer;

	//generate a buffer for the d.Suliman:
	glGenBuffers(1, &buffer);

	//select the working buffer or something like this:
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	//charge the buffer with data:
	glBufferData(GL_ARRAY_BUFFER, 4*2 * sizeof(float), position, GL_STATIC_DRAW);

	//enable the first attribute 0:
	glEnableVertexAttribArray(0);

	//determine the layout of the buffer that the gpu will read data from:
	//when we sit the layout the the vertex array object will be linked to the current bound array buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//index buffer object initilization:
	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	
	//read the shader code from the basic.shader file:
	ShaderProgramSource source = ParseShader("res/shaders/basic.shader");

	//create the shader:
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

	//use the shader:
	glUseProgram(shader);

	//get the id of the uniform form the gpu to use it:
	GlCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);

	//assign the color values to the uniform inside gpu memory:
	GlCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

	//unbound every buffer and shader:
	GlCall(glBindVertexArray(0));
	GlCall(glUseProgram(0));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float red = 0.0f;
	float delta = 0.05f;
	// حلقة العرض الرئيسية
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		//clear the previous frame for a clean screen for drawing:
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw the rectangle without use indecies buffer and but 6 as the number of vertex:
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		GlCall(glUseProgram(shader));

		//animate the color of the rectangle:
		GlCall(glUniform4f(location,red, 0.3, 0.8, 1.0f));
		GlCall(glBindVertexArray(vao));
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		//Draw the rectangle using indecies buffer and but 6 as the number of inecies not the vertex:
		GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		if (red > 1.0f) {
			delta = -0.005f;
		}
		else if (red < 0.0f) {
			delta = 0.05f;
		}

		red += delta;

		//swapt the previous and current frame:
		glfwSwapBuffers(window);

		//for process keyboard and other event without stop the program:
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	//free up resourses and close all related windows and do some cleanup:
	glfwTerminate();
	return 0;
}
