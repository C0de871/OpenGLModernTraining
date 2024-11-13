#pragma once

#include <glad/glad.h>

#define ASSERT(x) if (!(x)) __debugbreak();

//error handling for opengl errors that can't be seen by the compiler:
#define GlCall(x) GlClearError();\
	x;\
	ASSERT(GlLogCall(#x,__FILE__,__LINE__))

//clear all previous errors:
void GlClearError();

//log the error message to the consol 
bool GlLogCall(const char* function, const char* file, int line);
