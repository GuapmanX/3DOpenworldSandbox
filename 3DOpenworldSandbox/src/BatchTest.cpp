#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Addons/stb_image/stb_image.h"

#include <string>
#include <fstream>
#include <sstream>
#include <array>

#include "RendererClasses/Renderer.h"
#include "RendererClasses/IndexBuffer.h"
#include "RendererClasses/VertexBufferLayout.h"
#include "RendererClasses/VertexBuffer.h"
#include "RendererClasses/VertexArray.h"
#include "RendererClasses/Shader.h"
#include "RendererClasses/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui\imgui_impl_opengl3.h"
#include "imgui\imgui_impl_glfw.h"

struct Vertex
{
    float Position[2];
    float Color[4];
    float TexCoords[2];
    float TexID;
};
//std::array<Vertex, 4>


int main(void)
{


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;

    int HeightX = 1920;//640;
    int HeightY = 1080;//480;
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
            -50.0f, -50.0f, 0.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f,//0
             50.0f, -50.0f, 1.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, //1
             50.0f,  50.0f, 1.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, //2
            -50.0f,  50.0f, 0.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f,  //3

             100.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, //0
             200.0f, 100.0f, 1.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, //1
             200.0f, 200.0f, 1.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, //2
             100.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f  //3
        };*/

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
        };


        GLCall(glEnable(GL_BLEND)); //Enables blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //tells what the blended pixels should look like


        VertexArray va;
        VertexBuffer vb(nullptr, 8 * 9 * sizeof(float), GL_DYNAMIC_DRAW);

        VertexBufferLayout layout;
        layout.Push<float>(2); //position
        layout.Push<float>(2); //texture coordinates
        layout.Push<float>(4); //color
        layout.Push<float>(1); // texture differentiator
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 12);

        //glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);

        //glm::mat4 proj2 = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 0.9f);





        Shader shader("res/shaders/BatchRendering.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        //shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/ANGRY.PNG");
        Texture texture2("res/textures/code.PNG");

        texture.Bind(0);
        texture2.Bind(1);

        int samplers[2] = { 0, 1 };
        shader.SetUniform1iv("u_Textures", 2, samplers);
        //shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        glm::vec3 TranslationA(0, 0, 0);
        glm::vec2 ANGRYPOS(0, 0);
        glm::vec2 CODEPOS(0, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */

            //Set dynamic vertex buffer
            float positions[] = {
            -50.0f + ANGRYPOS.x, -50.0f + ANGRYPOS.y, 0.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f,//0
             50.0f + ANGRYPOS.x, -50.0f + ANGRYPOS.y, 1.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, //1
             50.0f + ANGRYPOS.x,  50.0f + ANGRYPOS.y, 1.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, //2
            -50.0f + ANGRYPOS.x,  50.0f + ANGRYPOS.y, 0.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f,  //3

             100.0f + CODEPOS.x, 100.0f + CODEPOS.y, 0.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, //0
             200.0f + CODEPOS.x, 100.0f + CODEPOS.y, 1.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, //1
             200.0f + CODEPOS.x, 200.0f + CODEPOS.y, 1.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, //2
             100.0f + CODEPOS.x, 200.0f + CODEPOS.y, 0.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f  //3
            };


            vb.Bind();
            //GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * 9 * sizeof(float),positions));
            vb.SetBufferData(positions);
            vb.Unbind();

            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();



            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), TranslationA);
                glm::mat4 mvp = proj * view * model;
                //shader.SetUniform4f("u_Color", 0.8f, 0.6f, 0.8f, 1.0f);
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }



            /* {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), TranslationB);
                glm::mat4 mvp = proj * view * model;
                //shader.SetUniform4f("u_Color", 0.8f, 0.6f, 0.8f, 1.0f);
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }*/





            //shader.Bind();
            //shader.SetUniform4f("u_Color", 0.8f, 0.6f, 0.8f, 1.0f);
           // shader.SetUniformMat4f("u_MVP", mvp);



            ImGui::Begin("Hello, world!");
            ImGui::SliderFloat2("TranslationA", &TranslationA.x, 0.0f, 960.0f);
            ImGui::SliderFloat2("Part1", &ANGRYPOS.x, 0.0f, 960.0f);
            ImGui::SliderFloat2("Part2", &CODEPOS.x, 0.0f, 960.0f);
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