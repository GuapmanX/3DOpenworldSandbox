#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Addons/stb_image/stb_image.h"

#include <string>
#include <fstream>
#include <sstream>

#include "RendererClasses/Renderer.h"
#include "RendererClasses/IndexBuffer.h"
#include "RendererClasses/VertexBufferLayout.h"
#include "RendererClasses/VertexBuffer.h"
#include "RendererClasses/VertexArray.h"
#include "RendererClasses/Shader.h"
#include "RendererClasses/Texture.h"
#include "RendererClasses/CubeMap.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui\imgui_impl_opengl3.h"
#include "imgui\imgui_impl_glfw.h"

#include "GameClasses/Camera.h"

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
    Vector3 Position{ 0.0f ,0.0f, 0.0f};
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

static Cube CreateCube(float x, float y, float z, float size )
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
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        std::cout << "respect" << std::endl;
}

struct Time
{
    float LastTime = glfwGetTime();


    float GetDeltaTime()
    {
        float Current = glfwGetTime();
        float Delta = Current - LastTime;
        LastTime = Current;
        return Delta;
    }
};


//GLOBAL VARIABLES
Camera MainCamera;
Time DT;
//

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

        /*float positions[] = {
            // front
                   -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0
                   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // 1
                   0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   // 2
                   -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // 3

                   // top
                   -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 4
                   0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  // 5
                   0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // 6
                   -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // 7

                   // left
                   -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 8
                   -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // 9
                   -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // 10
                   -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // 11

                   // right
                   0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, // 12
                   0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,  // 13
                   0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,   // 14
                   0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,  // 15

                   // back
                   -0.5f, -0.5f, 0.5f, 0.7f, 0.7f, 0.7f, // 16
                   0.5f, -0.5f, 0.5f, 0.7f, 0.7f, 0.7f,  // 17
                   0.5f, 0.5f, 0.5f, 0.7f, 0.7f, 0.7f,   // 18
                   -0.5f, 0.5f, 0.5f, 0.7f, 0.7f, 0.7f,  // 19

                   // bottom
                   -0.5f, -0.5f, 0.5f, 0.3f, 0.3f, 0.3f, // 20
                   0.5f, -0.5f, 0.5f, 0.3f, 0.3f, 0.3f,  // 21
                   0.5f, -0.5f, -0.5f, 0.3f, 0.3f, 0.3f, // 22
                   -0.5f, -0.5f, -0.5f, 0.3f, 0.3f, 0.3f // 23
        };*/

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


        GLCall(glEnable(GL_BLEND)); //Enables blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //tells what the blended pixels should look like

        /* These lines make sure that  the closest vertices are always in front of the farthest vertices in the z - axis*/
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LESS));

        VertexArray va;
        VertexBuffer vb(&positions, sizeof(Cube));

        VertexBufferLayout layout;
        layout.Push<float>(3);//position
        layout.Push<float>(3);//color
        layout.Push<float>(2);//texcoords
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 36);






        Shader shader("res/shaders/3DObjectCM.shader");
        shader.Bind();

        //shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        //shader.SetUniformMat4f("u_MVP", mvp);



        //Texture texture("res/textures/ANGRY.PNG");
        //texture.Bind(0);
        //shader.SetUniform1i("u_Texture", 0);
        


        std::array<std::string, 6> textures = {
            "res/textures/GrassBlockSide.png", //RIGHT
            "res/textures/GrassBlockSide.png", //LEFT
            "res/textures/GrassBlockTop.png",//TOP
            "res/textures/GrassBlockBottom.png", //BOTTOM
            "res/textures/GrassBlockSide.png", //FRONT
            "res/textures/GrassBlockSide.png", //REAR
        };

        CubeMap CM(textures);
        CM.Bind(0);
        shader.SetUniform1i("u_Texture", 0);



        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;


        glm::vec3 ObjectRotation(0.0f, 0.0f, 0.0f);


        double centerX = HeightX / 2.0;
        double centerY = HeightY / 2.0;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {


            ///////CAMERA SYSTEM//////////////
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            double deltaX = mouseX - centerX;
            double deltaY = mouseY - centerY;

            MainCamera.AddRotation(deltaX, deltaY);
            glfwSetCursorPos(window, centerX, centerY);
            ///////////////////////////////////


            /* Render here */
            GLCall(glClearColor(0.3f, 0.3f, 0.3f, 0.0f));
            renderer.Clear();


            processInput(window);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();



            {
                //shit projection
                /*glm::mat4 modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::rotate(modelMatrix, glm::radians(ObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(ObjectRotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(ObjectRotation.z), glm::vec3(0.0f, 1.0f, 0.0f));*/

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::rotate(model, glm::radians(ObjectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(ObjectRotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::rotate(model, glm::radians(ObjectRotation.z), glm::vec3(0.0f, 1.0f, 0.0f));

                //glm::mat4 view = glm::mat4(1.0f);

               /* CameraRotation Rotation = MainCamera.GetRotation();


                glm::vec3 direction;

                direction.x = cos(glm::radians(Rotation.X)) * cos(glm::radians(-Rotation.Y));
                direction.y = sin(glm::radians(-Rotation.Y));
                direction.z = sin(glm::radians(Rotation.X)) * cos(glm::radians(-Rotation.Y));

                direction = glm::normalize(direction);*/

                MainCamera.SetPosition(MainCamera.GetPosition() + (MainCamera.GetDirection() * DT.GetDeltaTime()));

                MainCamera.Update();

                //CameraPosition += direction * DT.GetDeltaTime();

                //iew = glm::rotate(view, glm::radians(Rotation.Y), glm::vec3(1.0f, 0.0f, 0.0f));
                //view = glm::rotate(view, glm::radians(Rotation.X), glm::vec3(0.0f, 1.0f, 0.0f));

                //view = glm::translate(view, CameraPosition);
                //glm::vec3 cameraTarget = CameraPosition + direction;
               // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
                //glm::mat4 view = glm::lookAt(CameraPosition, cameraTarget, up);

                glm::mat4 view = MainCamera.GetViewMatrix();

                glm::mat4 projection;
                projection = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);

                glm::mat4 mvp = projection * view * model;


                //shader.SetUniform4f("u_Color", 0.8f, 0.6f, 0.8f, 1.0f);
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }





            //shader.Bind();
            //shader.SetUniform4f("u_Color", 0.8f, 0.6f, 0.8f, 1.0f);
           // shader.SetUniformMat4f("u_MVP", mvp);

            va.Bind();
            ib.Bind();

            // renderer.Draw(va, ib, shader);

            ImGui::Begin("Hello, world!");
            ImGui::SliderFloat3("Block Rotation", &ObjectRotation.x, 0.0f, 360.0f);
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