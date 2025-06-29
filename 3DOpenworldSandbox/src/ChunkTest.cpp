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

#include "Shapes/Cube.h"
#include "Texture.h"







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


        const int ChunkWidth = 16;
        const int ChunkHeight = 32;
        unsigned int* Indices = new unsigned int[ChunkWidth * ChunkWidth * ChunkHeight * 36];
    {
        GLCall(glEnable(GL_BLEND)); //Enables blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //tells what the blended pixels should look like

        /* These lines make sure that  the closest vertices are always in front of the farthest vertices in the z - axis*/
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LESS));

        VertexArray va;


        VertexBuffer vb(nullptr, sizeof(Cube) * ChunkWidth * ChunkWidth * ChunkHeight, GL_DYNAMIC_DRAW);


        /*int arr[5];
        std::vector<int> v_arr;
        v_arr.push_back(5);
        v_arr.push_back(6);
        v_arr.push_back(7);
        v_arr.push_back(8);
        v_arr.push_back(9);
        v_arr.push_back(5);
        v_arr.push_back(6);
        v_arr.push_back(7);
        v_arr.push_back(8);
        v_arr.push_back(9);

        std::cout << sizeof(arr) << std::endl;
        std::cout << sizeof(v_arr) << std::endl;*/
        Cube CubeForChunk = CreateCube(0.0f, 0.0f, 0.0f, 0.5f);
        Cube CubeForChunk2 = CreateCube(1.0f, 1.0f, 0.0f, 0.5f);

        //vb.SetBufferData(&CubeForChunk,0,sizeof(Cube));
        vb.SetBufferData(&CubeForChunk2, 0, sizeof(Cube));
        
        VertexBufferLayout vbl;
        vbl.Push<float>(3);
        vbl.Push<float>(3);
        vbl.Push<float>(2);
        vbl.Push<float>(3);
        vbl.Push<float>(1);

        va.AddBuffer(vb, vbl);

        
        for (int face = 0; face < ChunkWidth * ChunkWidth * ChunkHeight * 6; face++)
        {
            Indices[0 + face * 6] = 0 + face * 4;
            Indices[1 + face * 6] = 1 + face * 4;
            Indices[2 + face * 6] = 2 + face * 4;
            Indices[3 + face * 6] = 2 + face * 4;
            Indices[4 + face * 6] = 3 + face * 4;
            Indices[5 + face * 6] = 0 + face * 4;
        }

        IndexBuffer IB(Indices, ChunkWidth * ChunkWidth * ChunkHeight * 36);

        Shader CubeShader("res/shaders/3DObjectRegTex.shader");



        //CM.Bind(0);
        //shader.SetUniform1i("u_Texture", 0);
        //CubeShader.Bind();

        //CubeMap Textures(textures);
        //Textures.Bind(0);
        //CubeShader.SetUniform1i("u_Texture", 0);


        CubeShader.Bind();
        std::array<std::string, 6> textureFiles = {
            "res/textures/GrassBlockSide.png", //RIGHT
            "res/textures/GrassBlockSide.png", //LEFT
            "res/textures/GrassBlockTop.png",//TOP
            "res/textures/GrassBlockBottom.png", //BOTTOM
            "res/textures/GrassBlockSide.png", //FRONT
            "res/textures/GrassBlockSide.png", //REAR
        };

        //std::array<Texture, 6> Textures;

        /*for (int i = 0; i < Textures.size(); i++) {
            Textures[i] = Texture(textureFiles[i]);
            Textures[i].Bind(i);
        }*/
        Texture Tex1(textureFiles[0]);
        Texture Tex2(textureFiles[1]);
        Texture Tex3(textureFiles[2]);
        Texture Tex4(textureFiles[3]);
        Texture Tex5(textureFiles[4]);
        Texture Tex6(textureFiles[5]);

        Tex1.Bind(0);
        Tex2.Bind(1);
        Tex3.Bind(2);
        Tex4.Bind(3);
        Tex5.Bind(4);
        Tex6.Bind(5);

        //Texture TextureOne("res/textures/GrassBlockSide.png");
        //Texture TextureTwo("res/textures/GrassBlockTop.png");

        //TextureOne.Bind(0);
        //TextureOne.Bind(1);



        int samplers[6] = { 0,1,2,3,4,5 };
        CubeShader.SetUniform1iv("u_Textures", 6, samplers);
       // CubeShader.SetUniform1i("u_Texture", 0);



        va.Unbind();
        CubeShader.Unbind();
        vb.Unbind();
        IB.Unbind();

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

            glm::mat4 View = Camera::GetViewMatrix();
            glm::mat4 Projection = Camera::GetProjection();
            glm::mat4 Model = glm::mat4(1.0f);

            //glm::mat4 mvp = Projection * View * Model;


            //m_Shader->ExcecuteShader();



            //m_Shader->SetUniformMat4f("u_MVP", mvp);
            CubeShader.Bind();
            CubeShader.SetUniformMat4f("Model", Model);
            CubeShader.SetUniformMat4f("View", View);
            CubeShader.SetUniformMat4f("Projection", Projection);
            CubeShader.Unbind();

            renderer.Draw(va, IB, CubeShader);







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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}