#include "GLFW/glfw3.h"

float LastTime = glfwGetTime();
float DeltaTime = 0.0f;

namespace Time
{
    void UpdateDeltaTime()
    {
        float Current = glfwGetTime();
        DeltaTime = Current - LastTime;
        LastTime = Current;
    }

    float GetDeltaTime()
    {
        return DeltaTime;
    }
}