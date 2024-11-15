//a buffer is a data we store in the memory
//a gl buffer is a data we store in vram of gpu

//a shader is a program that run in the gpu
//a vertix shader tell the gpu where the vertix should be in the screen and will be called for each vertix
//a fragment shader will draw each pixel that should be drawn and decide the color of it and will be called for each pixel

//a vertix attribute pointer is used to tell the gpu what each part of the buffer represent weither it is a position or texture or float and so on...

//a vertix contain way more data than position like color and so on...

//--------------------------------------------------//
// Specifies the location of the attribute in the shader.
// This is usually set by `layout(location = index)` in the vertex shader or via glBindAttribLocation.
// For example, if `index` is 0, this will refer to the attribute at location 0 in the shader.
//GLuint index;

// Specifies the number of components in the attribute (e.g., x, y for a 2D position).
// Possible values are 1, 2, 3, or 4, depending on the data in your shader.
// Example: For a vec3 attribute (e.g., position in 3D), you would use 3 here.
//GLint size;

// Specifies the data type of each component in the attribute.
// Common types include:
// - GL_FLOAT for floats
// - GL_INT for integers
// - GL_UNSIGNED_BYTE for unsigned bytes (e.g., color values).
// This should match the data type used in the shader attribute.
//GLenum type;

// Specifies whether the data should be normalized (converted to a range between 0 and 1 or -1 and 1).
// If GL_TRUE, data will be normalized. If GL_FALSE, data will be used as-is.
// Normalization is often used for attributes like colors where you want byte data (0-255) scaled.
//GLboolean normalized;

// Specifies the byte offset between consecutive vertex attributes (stride).
// If the attribute data is tightly packed, this can be set to 0, and OpenGL will calculate the stride automatically.
// Otherwise, you can set it to the total size of the vertex structure to move from one vertex to the next.
//GLsizei stride;

// Specifies the offset in the buffer where the first component of this attribute is located.
// This is often `0` if the attribute is at the start of the vertex structure.
// For attributes further in the structure, set this to the offset in bytes.
//const void* pointer;
//--------------------------------------------------//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include <chrono>
#include "Shader.h";


// دالة تعالج تغيير حجم النافذة
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// دالة لمعالجة إدخال المستخدم (إغلاق النافذة عند الضغط على مفتاح Escape)
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
	glfwSwapInterval(0);

	// تحميل دوال OpenGL باستخدام GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}





	{
		//our vertex and indices//
		//------------------------------//
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
		//------------------------------//
		//end of the init//






		VertexArray va;
		VertexBuffer vb(position, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);








		Shader shader("res/shaders/basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


		va.UnBind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();


		float red = 0.0f;
		float delta = 1.5f;
		auto lastTime = std::chrono::high_resolution_clock::now();

		bool first = true;
		Renderer renderer;


		// حلقة العرض الرئيسية
		// Main rendering loop
		while (!glfwWindowShouldClose(window)) {
			processInput(window);

			renderer.Clear();

			// Calculate deltaTime
			auto currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> elapsed = currentTime - lastTime;
			float deltaTime = elapsed.count();
			lastTime = currentTime;

			// Update color with deltaTime
			red += delta * deltaTime;  // Adjust color smoothly based on deltaTime
			if (first) {
				std::cout << red << std::endl;
				std::cout << deltaTime << std::endl;
				first = false;
			}
			if (red > 1.0f) {
				red = 1.0f;
				delta = -delta;
			}
			else if (red < 0.0f) {
				red = 0.0f;
				delta = -delta;
			}

			// Bind and use the shader
			shader.Bind();
			shader.SetUniform4f("u_Color", red, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va,ib,shader);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	//free up resourses and close all related windows and do some cleanup:
	glfwTerminate();
	return 0;
}
