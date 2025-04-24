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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui\imgui_impl_opengl3.h"
#include "imgui\imgui_impl_glfw.h"

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
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, //0
             50.0f, -50.0f, 1.0f, 0.0f, //1
             50.0f,  50.0f, 1.0f, 1.0f, //2
            -50.0f,  50.0f, 0.0f, 1.0f  //3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND)); //Enables blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //tells what the blended pixels should look like


        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        //glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

        //glm::mat4 proj2 = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 0.9f);





        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        //shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/ANGRY.PNG");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        glm::vec3 TranslationA(200, 200, 0);
        glm::vec3 TranslationB(400, 200, 0);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
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



            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), TranslationB);
                glm::mat4 mvp = proj * view * model;
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

            renderer.Draw(va, ib, shader);

            ImGui::Begin("Hello, world!");
            ImGui::SliderFloat2("TranslationA", &TranslationA.x, 0.0f, 960.0f);
            ImGui::SliderFloat2("TranslationB", &TranslationB.x, 0.0f, 960.0f);
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