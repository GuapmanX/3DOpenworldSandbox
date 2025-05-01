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

struct Vector2
{
    float x, y;
};

struct RGBA
{
    float r, g, b, a;
};

struct TextureCoordinate
{
    float x, y;
};


struct Vertex
{
    Vector2 Position;
    TextureCoordinate texCoords;
    RGBA Color;
    float TexID;
};

static std::array<Vertex, 4> CreateQuad(float x, float y, float size, float textureID)
{

    Vertex v0;
    v0.Position = { x , y };
    v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v0.texCoords = { 0.0f, 0.0f };
    v0.TexID = textureID;

    Vertex v1;
    v1.Position = { x + size, y };
    v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v1.texCoords = { 1.0f, 0.0f };
    v1.TexID = textureID;

    Vertex v2;
    v2.Position = { x + size, y + size };
    v2.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v2.texCoords = { 1.0f, 1.0f };
    v2.TexID = textureID;

    Vertex v3;
    v3.Position = { x, y + size };
    v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v3.texCoords = { 0.0f, 1.0f };
    v3.TexID = textureID;

    return { v0, v1, v2, v3 };
};

static Vertex* AddQuad(Vertex* target,float x, float y, float size, float textureID)
{

    target->Position = { x , y };
    target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    target->texCoords = { 0.0f, 0.0f };
    target->TexID = textureID;

    target++;

    target->Position = { x + size, y };
    target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    target->texCoords = { 1.0f, 0.0f };
    target->TexID = textureID;

    target++;

    target->Position = { x + size, y + size };
    target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    target->texCoords = { 1.0f, 1.0f };
    target->TexID = textureID;

    target++;

    target->Position = { x, y + size };
    target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    target->texCoords = { 0.0f, 1.0f };
    target->TexID = textureID;

    target++;

    return target;
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
        const unsigned int MaxQuadCount = 1000;
        const unsigned int MaxVertexCount = MaxQuadCount * 4;
        const unsigned int MaxIndexCount = MaxQuadCount * 6;


        unsigned int indices[MaxIndexCount];
        unsigned int offset = 0;
        for (unsigned int i = 0; i < MaxIndexCount; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;

            offset += 4;
        }

        GLCall(glEnable(GL_BLEND)); //Enables blending
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //tells what the blended pixels should look like


        VertexArray va;
        VertexBuffer vb(nullptr, MaxVertexCount * 9 * sizeof(float), GL_DYNAMIC_DRAW);

        VertexBufferLayout layout;
        layout.Push<float>(2); //position
        layout.Push<float>(2); //texture coordinates
        layout.Push<float>(4); //color
        layout.Push<float>(1); // texture differentiator
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, MaxIndexCount);

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);





        Shader shader("res/shaders/BatchRendering.shader");
        shader.Bind();


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
            unsigned int indexCount = 0;

            std::array<Vertex, 1000> vertices;
            Vertex* buffer = vertices.data();
            for (int y = 0; y < 5; y++)
            {

                for (int x = 0; x < 5; x++)
                {
                    buffer = AddQuad(buffer, x * 100.0f, y * 100.0f, 100.0f, (x + y) % 2);
                    indexCount += 6;
                }
            }

            buffer = buffer = AddQuad(buffer, ANGRYPOS[0], ANGRYPOS[1], 100.0f, 0.0f);



            vb.SetBufferData(vertices.data(), 0, vertices.size() * 9 * sizeof(float));
            indexCount += 6;

             //auto q0 = CreateQuad(-50.0f + ANGRYPOS.x, -50.0f + ANGRYPOS.y, 100.0f, 0.0f);
            // auto q1 = CreateQuad(100.0f + CODEPOS.x, 100.0f + CODEPOS.y, 100.0f, 1.0f);

             //Vertex vertices[8];
             //memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
             //memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

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