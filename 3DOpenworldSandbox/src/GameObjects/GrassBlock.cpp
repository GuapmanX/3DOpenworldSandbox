#include "GrassBlock.h"
#include "Shapes/Cube.h"
#include <array>

unsigned int IBBuffer[36];
std::vector<unsigned int> Layout = { 3, 3, 2 };
Cube Ps = CreateCube(0.0f, 0.0f, 0.0f, 0.5f);

bool Populated = PopulateCubeIndexBuffer(IBBuffer);
BufferObject GB_BUFFER(&Ps, sizeof(Cube), IBBuffer, 36, Layout);



GrassBlock::GrassBlock()
{
	m_RenderBlock.addBufferObject(&GB_BUFFER);

}

GrassBlock::~GrassBlock()
{
}

void GrassBlock::Update(float deltaTime)
{

}
