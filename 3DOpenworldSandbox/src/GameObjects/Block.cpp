#include "Block.h"
#include "Shapes/Cube.h"
#include <array>
#include "glm/gtc/matrix_transform.hpp"
#include "GameCore/Camera.h"

static BufferObject GB_BUFFER;
static Shader GB_SHADER;

static std::array<std::string, 6> GB_FACES = {
    "res/textures/GrassBlockSide.png", //RIGHT
    "res/textures/GrassBlockSide.png", //LEFT
    "res/textures/GrassBlockTop.png",//TOP
    "res/textures/GrassBlockBottom.png", //BOTTOM
    "res/textures/GrassBlockSide.png", //FRONT
    "res/textures/GrassBlockSide.png", //REAR
};

void InitBuffer()
{
	//unsigned int IBBuffer[36];
    //bool Populated = PopulateCubeIndexBuffer(IBBuffer);

	unsigned int IBBuffer[] = {
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
	std::vector<unsigned int> Layout = { 3, 3, 2, 3, 2 };
	Cube Ps = CreateCube(0.0f, 0.0f, 0.0f, 0.5f);

	GB_BUFFER = BufferObject(&Ps, sizeof(Cube), IBBuffer, 36, Layout);
	GB_SHADER = Shader("res/shaders/3DObjectCM.shader");
}


Block::Block(const glm::vec3& Position)
    : m_ModelMatrix(1.0f), m_Position(Position), m_Rotation(glm::vec3(0.0f,0.0f,0.0f))
{
	m_RenderBlock.addBufferObject(&GB_BUFFER);
	m_RenderBlock.addShaderObject(&GB_SHADER);
    
    Shader& SHADER = m_RenderBlock.GetShader();

    SHADER.Bind();
    m_cubeMap = CubeMap(GB_FACES);
    m_cubeMap.Bind(0);
    SHADER.SetUniform1i("u_Texture", 0);
    SHADER.Unbind();


	//BufferObject GB_BUFFER;//(&Ps, sizeof(Cube), IBBuffer, 36, Layout);
	//GB_BUFFER = BufferObject(&Ps, sizeof(Cube), IBBuffer, 36, Layout);

}

Block::~Block()
{
}

void Block::Update(const float& deltaTime)
{
    m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position);
    m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 0.0f, 1.0f));
    m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 1.0f, 0.0f));

	m_RenderBlock.Draw(deltaTime,m_ModelMatrix);
}

void Block::ChangeTexture(const std::array<std::string, 6>& FACES)
{
    Shader& SHADER = m_RenderBlock.GetShader();

    SHADER.Bind();
    m_cubeMap.Unbind();
    m_cubeMap.~CubeMap();
    m_cubeMap = CubeMap(FACES);
    m_cubeMap.Bind(0);
    SHADER.SetUniform1i("u_Texture", 0);
    SHADER.Unbind();
}

void Block::ApplyLighting(const glm::vec4& SourceColor, glm::vec3& SourcePosition)
{
    Shader& SHADER = m_RenderBlock.GetShader();
    SHADER.Bind();
    float a = 1 / glm::length(SourcePosition - m_Position);

    glm::vec3 CamPos = Camera::GetPosition();
    //SHADER.SetUniform3f("u_CameraPosition",CamPos.x,CamPos.y,CamPos.z);

    //SHADER.SetUniform3f("u_LightPosition", SourcePosition.x, SourcePosition.y, SourcePosition.z);
    //SHADER.SetUniform1f("u_LightDistance",a);
    //SHADER.SetUniform4f("u_LightColor", SourceColor.r,SourceColor.g,SourceColor.b,SourceColor.a);
    SHADER.Unbind();
}
