#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Addons/stb_image/stb_image.h"

#include <string>
#include <fstream>
#include <sstream>

#include "GameClasses/BufferObject.h"
#include "GameClasses/ShaderObject.h"
#include "GameClasses/RenderObject.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui\imgui_impl_opengl3.h"
#include "imgui\imgui_impl_glfw.h"

#include "GameCore/Camera.h"
#include "GameCore/Time.h"
#include <functional>


#include "GameObjects/Block.h"
#include "GameObjects/Light_Block.h"







void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::SetPosition(Camera::GetPosition() + (Camera::GetDirection() * Time::GetDeltaTime() * Camera::GetCameraSpeed()));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::SetPosition(Camera::GetPosition() - (Camera::GetDirection() * Time::GetDeltaTime() * Camera::GetCameraSpeed()));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::SetPosition(Camera::GetPosition() - (glm::cross(Camera::GetDirection(), upVector) * Time::GetDeltaTime() * Camera::GetCameraSpeed()));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::SetPosition(Camera::GetPosition() + (glm::cross(Camera::GetDirection(), upVector) * Time::GetDeltaTime() * Camera::GetCameraSpeed()));
};


int main(void)
{


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;

    int HeightX = 960;//640;
    int HeightY = 540;//480;
    const char* WindowName = "Window";
    GLFWmonitor* monitor = glfwGetPrimaryMonitor(); //Goes into the 4th slot of glfwCreateWindow


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(HeightX, HeightY, WindowName, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);//locks the framerate to monitors refresh rate(2 would be 2x slower than monitors refresh rate)


    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    InitBuffer();
    InitLightBuffer();

    std::cout << glGetString(GL_VERSION) << std::endl;


    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();


    {
        GLCall(glEnable(GL_BLEND)); //Enables blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //tells what the blended pixels should look like

        /* These lines make sure that  the closest vertices are always in front of the farthest vertices in the z - axis*/
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LESS));





        /*std::vector<unsigned int> Layout = {3, 3, 2};

        BufferObject MinecraftBlockVB;
        MinecraftBlockVB = BufferObject(&positions, sizeof(Cube), indices, 36, Layout);


        std::array<std::string, 6> texturesCB = {
            "res/textures/GrassBlockSide.png", //RIGHT
            "res/textures/GrassBlockSide.png", //LEFT
            "res/textures/GrassBlockTop.png",//TOP
            "res/textures/GrassBlockBottom.png", //BOTTOM
            "res/textures/GrassBlockSide.png", //FRONT
            "res/textures/GrassBlockSide.png", //REAR
        };

        CubeMap CM(texturesCB);


        Shader ShaderOBJ;
        ShaderOBJ = Shader("res/shaders/3DObjectCM.shader");

        ShaderOBJ.Bind();
        CM.Bind(0);
        ShaderOBJ.SetUniform1i("u_Texture", 0);
        ShaderOBJ.Unbind();

        RenderObject MCBLOCK(&MinecraftBlockVB, &ShaderOBJ);*/

        Block FirstBlock(glm::vec3(1.0f,1.0f,1.0f));
        LightBlock LightSource(glm::vec3(-2.0f, -2.0f, -2.0f));
        LightSource.SetLightColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

        
        Renderer renderer;


        double centerX = HeightX / 2.0;
        double centerY = HeightY / 2.0;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            Time::UpdateDeltaTime();

            
            //std::cout << DT.GetDeltaTime() << std::endl;

            ///////CAMERA SYSTEM//////////////
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            double deltaX = mouseX - centerX;
            double deltaY = mouseY - centerY;

            Camera::AddRotation(deltaX, deltaY);
            glfwSetCursorPos(window, centerX, centerY);
            ///////////////////////////////////


            /* Render here */
            GLCall(glClearColor(0.3f, 0.3f, 0.3f, 0.0f));
            renderer.Clear();
             

            processInput(window);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();



            Camera::Update();

            FirstBlock.m_Position = glm::vec3(cos(glfwGetTime()) * 3, cos(glfwGetTime()) * 3, cos(glfwGetTime()) * 3);
            FirstBlock.ApplyLighting(LightSource.GetColor(), LightSource.m_Position);
            FirstBlock.Update(Time::GetDeltaTime());

            LightSource.Update(Time::GetDeltaTime());






            ImGui::Begin("Hello, world!");
            ImGui::Text("framerate = %f",1.0f/Time::GetDeltaTime());
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}