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


#include "GameObjects/Block.h"
#include "GameObjects/Light_Block.h"

//#include "Shapes/Cube.h"
#include "Shapes/dynamicCube.h";
#include "Texture.h"
#include"GameObjects/Chunk.h"
#include "MIPMAP.h"

#include "GameObjects/Skybox.h"
#include "Managers/ChunkManager.h"




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

    //InitBuffer();
    //InitLightBuffer();

    std::cout << glGetString(GL_VERSION) << std::endl;


    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

        
        const int ChunkWidth = 16;
        const int ChunkHeight = 32;
        unsigned int* Indices = new unsigned int[ChunkWidth * ChunkWidth * ChunkHeight * 36];
    {
        GLCall(glEnable(GL_BLEND)); //Enables blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //tells what the blended pixels should look like

        /* These lines make sure that  the closest vertices are always in front of the farthest vertices in the z - axis*/
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LESS));

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        Initialize();

        /*std::vector<std::string> TEXDATA;
        TEXDATA.reserve(4);
        TEXDATA.push_back("Terrain.png");
        TEXDATA.push_back("Terrain_1.png");
        TEXDATA.push_back("Terrain_2.png");
        TEXDATA.push_back("Terrain_3.png");

        MIPMAP mpmp(TEXDATA);*/

        Skybox Box;

        /*Chunk Chuck( 0.0f, 0.0f, 0.0f ); 
        Chunk Chuck2(16.0f, 0.0f, 0.0f);

        float Start = glfwGetTime();
        for (int x = 1; x < ChunkWidth + 1; x++)
        {
            for (int y = 1; y < ChunkHeight + 1; y++)
            {
                for (int z = 1; z < ChunkWidth + 1; z++)
                {
                    Chuck.SetBlock(x, y, z);
                    Chuck2.SetBlock(x, y, z);
                }
            }
        }
        float End = glfwGetTime();
        std::cout << "Drawing a whole chunk took " << (End - Start) << "seconds" << std::endl;*/

        //std::cout << "X:" << ChunkWidth - 2 << "Y:" << ChunkHeight - 2 << "Z:" << ChunkWidth - 2 << std::endl;
        

        //Chuck.SetBlock(16, 32, 16);
        //bool isfull = Chuck.m_BlockMatrix[15][15][31].isEmpty();
        //std::cout << isfull << std::endl;
        //Chuck.SetBlock(ChunkWidth, ChunkHeight, ChunkWidth);
        set_chunk_buffer_size(30);


        

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


            //FirstBlock.Update(Time::GetDeltaTime());

            //glm::mat4 View = Camera::GetViewMatrix();
            //glm::mat4 Projection = Camera::GetProjection();
            //glm::mat4 Model = glm::mat4(1.0f);

            //glm::mat4 mvp = Projection * View * Model;


            //m_Shader->ExcecuteShader();



            //m_Shader->SetUniformMat4f("u_MVP", mvp);

            /*CubeShader.Bind();
            CubeShader.SetUniformMat4f("Model", Model);
            CubeShader.SetUniformMat4f("View", View);
            CubeShader.SetUniformMat4f("Projection", Projection);
            CubeShader.Unbind();

            renderer.Draw(va, IB, CubeShader);*/
            render_chunks();
            //Chuck.Render();
            //Chuck2.Render();
            Box.Render();






            ImGui::Begin("Hello, world!");
            ImGui::Text("framerate = %f", 1.0f / Time::GetDeltaTime());
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    delete[] Indices;
    Indices = nullptr;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}