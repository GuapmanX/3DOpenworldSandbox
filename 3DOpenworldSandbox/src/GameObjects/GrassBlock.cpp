#include "GrassBlock.h"
#include "Shapes/Cube.h"
#include <array>


BufferObject GB_BUFFER;
Shader GB_SHADER;

std::array<std::string, 6> GB_FACES = {
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
	std::vector<unsigned int> Layout = { 3, 3, 2 };
	Cube Ps = CreateCube(0.0f, 0.0f, 0.0f, 0.5f);

	GB_BUFFER = BufferObject(&Ps, sizeof(Cube), IBBuffer, 36, Layout);
	GB_SHADER = Shader("res/shaders/3DObjectCM.shader");
}


GrassBlock::GrassBlock()
{
	m_RenderBlock.addBufferObject(&GB_BUFFER);
	m_RenderBlock.addShaderObject(&GB_SHADER);
    
    Shader& tsh = m_RenderBlock.GetShader();

    tsh.Bind();
    CM = CubeMap(GB_FACES);
    CM.Bind(0);
    tsh.SetUniform1i("u_Texture", 0);
    tsh.Unbind();


	//BufferObject GB_BUFFER;//(&Ps, sizeof(Cube), IBBuffer, 36, Layout);
	//GB_BUFFER = BufferObject(&Ps, sizeof(Cube), IBBuffer, 36, Layout);

}

GrassBlock::~GrassBlock()
{
}

void GrassBlock::Update(float deltaTime)
{
	m_RenderBlock.Draw(deltaTime);
}
