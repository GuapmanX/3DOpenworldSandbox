#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Addons/stb_image/stb_image.h"

#include <string>
#include <fstream>
#include <sstream>

//#include "RendererClasses/Renderer.h"
//#include "RendererClasses/IndexBuffer.h"
//#include "RendererClasses/VertexBufferLayout.h"
//#include "RendererClasses/VertexBuffer.h"
//#include "RendererClasses/VertexArray.h"
//#include "RendererClasses/Shader.h"
//#include "RendererClasses/Texture.h"
//#include "RendererClasses/CubeMap.h"

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


struct Vector3
{
    float x, y, z;
};

struct RGB
{
    float R, G, B;
};

struct TextureCoordinates2D
{
    float x, y;
};



struct Vertex
{
    Vector3 Position{ 0.0f ,0.0f, 0.0f };
    RGB Color = { 1.0f, 0.0f, 0.0f };
    TextureCoordinates2D TC{ 0.0f, 0.0f };
};

struct Quad
{
    Vertex v0, v1, v2, v3;
};

struct Cube
{
    Quad q0, q1, q2, q3, q4, q5;
};

static Cube CreateCube(float x, float y, float z, float size)
{
    Cube C;

    //FRONT
    C.q0.v0.Position = { x - size, y - size, z - size };
    C.q0.v1.Position = { x + size, y - size, z - size };
    C.q0.v2.Position = { x + size, y + size, z - size };
    C.q0.v3.Position = { x - size, y + size, z - size };

    C.q0.v0.TC = { 0.0f, 0.0f };
    C.q0.v1.TC = { 1.0f, 0.0f };
    C.q0.v2.TC = { 1.0f, 1.0f };
    C.q0.v3.TC = { 0.0f, 1.0f };

    //TOP
    C.q1.v0.Position = { x - size, y + size, z - size };
    C.q1.v1.Position = { x + size, y + size, z - size };
    C.q1.v2.Position = { x + size, y + size, z + size };
    C.q1.v3.Position = { x - size, y + size, z + size };

    C.q1.v0.TC = { 0.0f, 0.0f };
    C.q1.v1.TC = { 1.0f, 0.0f };
    C.q1.v2.TC = { 1.0f, 1.0f };
    C.q1.v3.TC = { 0.0f, 1.0f };

    //LEFT
    C.q2.v0.Position = { x - size, y - size, z - size };
    C.q2.v1.Position = { x - size, y + size, z - size };
    C.q2.v2.Position = { x - size, y + size, z + size };
    C.q2.v3.Position = { x - size, y - size, z + size };

    C.q2.v0.TC = { 0.0f, 0.0f };
    C.q2.v1.TC = { 1.0f, 0.0f };
    C.q2.v2.TC = { 1.0f, 1.0f };
    C.q2.v3.TC = { 0.0f, 1.0f };


    //RIGHT
    C.q3.v0.Position = { x + size, y - size, z - size };
    C.q3.v1.Position = { x + size, y + size, z - size };
    C.q3.v2.Position = { x + size, y + size, z + size };
    C.q3.v3.Position = { x + size, y - size, z + size };

    C.q3.v0.TC = { 0.0f, 0.0f };
    C.q3.v1.TC = { 1.0f, 0.0f };
    C.q3.v2.TC = { 1.0f, 1.0f };
    C.q3.v3.TC = { 0.0f, 1.0f };

    //BACK
    C.q4.v0.Position = { x - size, y - size, z + size };
    C.q4.v1.Position = { x + size, y - size, z + size };
    C.q4.v2.Position = { x + size, y + size, z + size };
    C.q4.v3.Position = { x - size, y + size, z + size };

    C.q4.v0.TC = { 0.0f, 0.0f };
    C.q4.v1.TC = { 1.0f, 0.0f };
    C.q4.v2.TC = { 1.0f, 1.0f };
    C.q4.v3.TC = { 0.0f, 1.0f };

    //BOTTOM
    C.q5.v0.Position = { x - size, y - size, z + size };
    C.q5.v1.Position = { x + size, y - size, z + size };
    C.q5.v2.Position = { x + size, y - size, z - size };
    C.q5.v3.Position = { x - size, y - size, z - size };

    C.q5.v0.TC = { 0.0f, 0.0f };
    C.q5.v1.TC = { 1.0f, 0.0f };
    C.q5.v2.TC = { 1.0f, 1.0f };
    C.q5.v3.TC = { 0.0f, 1.0f };

    return C;
};



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



        Cube positions = CreateCube(0.0f, 0.0f, 0.0f, 0.5f);


        unsigned int indices[] = {
            // front
             0, 1, 2, // first triangle
             2, 3, 0, // second triangle

             // top
             4, 5, 6, // first triangle
             6, 7, 4, // second triangle

             // left
             8, 9, 10,  // first triangle
             10, 11, 8, // second triangle

             // right
             14, 13, 12, // 12, 13, 14, // first triangle
             12, 15, 14, // 14, 15, 12, // second triangle

             // back
             18, 17, 16, // 16, 17, 18, // first triangle
             16, 19, 18, // 18, 19, 16, // second triangle

             // bottom
             20, 21, 22, // first triangle
             22, 23, 20  // second triangle
        };

        std::vector<unsigned int> Layout = { 3, 3, 2 };

        BufferObject MinecraftBlockVB(&positions, sizeof(Cube), indices, 36, Layout);


        std::array<std::string, 6> texturesCB = {
            "res/textures/GrassBlockSide.png", //RIGHT
            "res/textures/GrassBlockSide.png", //LEFT
            "res/textures/GrassBlockTop.png",//TOP
            "res/textures/GrassBlockBottom.png", //BOTTOM
            "res/textures/GrassBlockSide.png", //FRONT
            "res/textures/GrassBlockSide.png", //REAR
        };

        CubeMap CM(texturesCB);


        std::function<void(Shader& SHADER)> shaderRuntime = [&](Shader& SHADER) {
            SHADER.Bind();


            float Colorval = glm::abs(glm::cos(glfwGetTime()));
            std::cout << Colorval << std::endl;
            SHADER.SetUniform4f("u_Color", Colorval,Colorval,Colorval,1);

            SHADER.Unbind();
        };

        std::function<void(Shader& SHADER)> shaderInitializer = [&](Shader& SHADER) {
            SHADER.Bind();
            CM.Bind(0);
            SHADER.SetUniform1i("u_Texture", 0);
            SHADER.Unbind();
         };

        ShaderObject ShaderOBJ("res/shaders/3DObjectCM.shader",shaderRuntime, shaderInitializer);
        RenderObject MCBLOCK(&MinecraftBlockVB, &ShaderOBJ);





     

       



        



        
        Renderer renderer;


        glm::vec3 ObjectRotation(0.0f, 0.0f, 0.0f);


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
            MCBLOCK.Draw(Time::GetDeltaTime());




            //shader.Bind();
            //shader.SetUniform4f("u_Color", 0.8f, 0.6f, 0.8f, 1.0f);
           // shader.SetUniformMat4f("u_MVP", mvp);



            // renderer.Draw(va, ib, shader);

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