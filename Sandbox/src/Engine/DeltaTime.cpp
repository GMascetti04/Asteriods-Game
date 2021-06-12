#include "xpch.h"
#include "DeltaTime.h"
#include "GLFW/glfw3.h"
#include "Engine/Debug/Profiler.h"

//returns the time in seconds since the start of the program
double DeltaTime::getGLFWseconds()
{
    return glfwGetTime();
}
