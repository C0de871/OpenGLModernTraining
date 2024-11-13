#include <glad/glad.h>
#include <iostream>
//clear all previous errors:
void GlClearError() {
	while (glGetError() != GL_NO_ERROR);
}

//log the error message to the consol 
bool GlLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGl Error code]: (" << error << ") \n" << "[the function that cause the error]: " << function << "\n[The path of the file that cause the error]: " << file << "\n[The line]: " << line << std::endl;
		return false;
	}
	return true;
}
