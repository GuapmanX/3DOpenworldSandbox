#include "Chunk.h"
#include "GameCore/Camera.h"
#include <iostream>
#include <vector>
#include <iterator>
#include "MIPMAP.h"

unsigned int* Indices = new unsigned int[ChunkWidth * ChunkWidth * ChunkHeight * 36];
VertexBufferLayout C_VBL;
IndexBuffer C_IB;
Shader C_shader;
MIPMAP C_Texture;
VertexArray C_VA;

void SetIndexBuffer()
{
	for (int face = 0; face < ChunkWidth * ChunkWidth * ChunkHeight * 6; face++)
	{
		Indices[0 + face * 6] = 0 + face * 4;
		Indices[1 + face * 6] = 1 + face * 4;
		Indices[2 + face * 6] = 2 + face * 4;
		Indices[3 + face * 6] = 2 + face * 4;
		Indices[4 + face * 6] = 3 + face * 4;
		Indices[5 + face * 6] = 0 + face * 4;
	}

	C_IB = IndexBuffer(Indices, ChunkWidth * ChunkWidth * ChunkHeight * 36);
}

void SetShader() 
{
	C_shader = Shader("res/shaders/3DObjectATLASEARTH.shader");
}

void Setlayout()
{
	C_VBL.Push<float>(3);
	C_VBL.Push<float>(3);
	C_VBL.Push<float>(2);
	C_VBL.Push<float>(3);

	//atlas data
	C_VBL.Push<float>(2); //texture position
	////////
}

void SetTexture()
{
	std::vector<std::string> TEXDATA;
	TEXDATA.reserve(5);
	TEXDATA.push_back("res/textures/terrain.png");
	TEXDATA.push_back("res/textures/terrain_2.png");
	TEXDATA.push_back("res/textures/terrain_3.png");
	TEXDATA.push_back("res/textures/terrain_4.png");
	TEXDATA.push_back("res/textures/terrain_5.png");


	C_Texture = MIPMAP(TEXDATA);
}

void Initialize() {
	SetIndexBuffer();
	SetShader();
	Setlayout();
	SetTexture();
}

Chunk::Chunk(float x, float y, float z)
{
	va.AddBuffer(Positions, C_VBL);
	C_shader.Bind();
	C_Texture.Bind(0);
	C_shader.SetUniform1i("u_Texture", 0);
	C_shader.Unbind();
	Move(x, y, z);
}

Chunk::~Chunk() {
	
}

Chunk::Chunk(Chunk&& other) noexcept
{
	std::copy(&other.m_BlockMatrix[0][0][0], &other.m_BlockMatrix[0][0][0] + ChunkWidth * ChunkWidth * ChunkHeight, &this->m_BlockMatrix[0][0][0]);
	this->Positions = other.Positions;
	this->Move(other.m_Position.x, other.m_Position.y, other.m_Position.z);
	this->va = other.va;
}



bool Chunk::CheckForBlock(int x, int y, int z)
{
	//Checks if its outside the chunk borders
	if (x < 0 || x + 1 > ChunkWidth) { return true; }
	if (y < 0 || y + 1 > ChunkHeight) { return true; }
	if (z < 0 || z + 1 > ChunkWidth) { return true; }
	//////////////////////////////////////////


	return m_BlockMatrix[x][z][y].isEmpty(); //Checks if theres a block in the specific space
}

void Chunk::Move(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;

	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
}

void Chunk::CheckNearbyBlocks(bool (&values)[6],int& faceCount, int x, int y, int z)
{
	bool IsTop = CheckForBlock(x, y + 1, z);
	faceCount += IsTop;
	values[Faces::Top] = IsTop;

	bool IsBottom = CheckForBlock(x, y - 1, z);
	faceCount += IsBottom;
	values[Faces::Bottom] = IsBottom;

	bool IsRight = CheckForBlock(x + 1, y, z);
	faceCount += IsRight;
	values[Faces::Right] = IsRight;

	bool IsLeft = CheckForBlock(x - 1, y, z);
	faceCount += IsLeft;
	values[Faces::Left] = IsLeft;

	bool IsFront = CheckForBlock(x, y, z + 1);
	faceCount += IsFront;
	values[Faces::Front] = IsFront;

	bool IsBack = CheckForBlock(x, y, z - 1);
	faceCount += IsBack;
	values[Faces::Back] = IsBack;
}

void Chunk::RedrawBlock(int x, int y, int z)
{

	//Checks if its outside the chunk borders
	if (x < 0 || x + 1 > ChunkWidth) { return; }
	if (y < 0 || y + 1 > ChunkHeight) { return; }
	if (z < 0 || z + 1 > ChunkWidth) { return; }
	////////////////////////////////////////

	if (m_BlockMatrix[x][z][y].isEmpty()) { return; }

	ClearBufferPosition(x, y, z);
	SetBlockBufferData(x, y, z, false);
}

void Chunk::RedrawNearbyBlocks(int x, int y, int z)
{
	RedrawBlock(x, y + 1, z);
	RedrawBlock(x, y - 1, z);
	RedrawBlock(x + 1, y, z);
	RedrawBlock(x - 1, y, z);
	RedrawBlock(x, y, z + 1);
	RedrawBlock(x, y, z - 1);
}

std::vector<Quad> Chunk::GenerateCube(float x, float y, float z, const bool render[6], const int faces)
{
	std::vector<Quad> Data;
	Data.reserve(faces);

	const float size = 0.5f;

	if (render[Faces::Front]) {
		Quad FrontFace;

		FrontFace.v0.Position = { x - size, y - size, z + size };
		FrontFace.v1.Position = { x + size, y - size, z + size };
		FrontFace.v2.Position = { x + size, y + size, z + size };
		FrontFace.v3.Position = { x - size, y + size, z + size };

		FrontFace.v0.TC = { 0.0f, 0.0f };
		FrontFace.v1.TC = { 1.0f, 0.0f };
		FrontFace.v2.TC = { 1.0f, 1.0f };
		FrontFace.v3.TC = { 0.0f, 1.0f };

		FrontFace.v0.NV = { 0.0f, 0.0f, -1.0f };
		FrontFace.v1.NV = { 0.0f, 0.0f, -1.0f };
		FrontFace.v2.NV = { 0.0f, 0.0f, -1.0f };
		FrontFace.v3.NV = { 0.0f, 0.0f, -1.0f };

		FrontFace.v0.AtlasData = { 3.0f, 15.0f };
		FrontFace.v1.AtlasData = { 3.0f, 15.0f };
		FrontFace.v2.AtlasData = { 3.0f, 15.0f };
		FrontFace.v3.AtlasData = { 3.0f, 15.0f };

		Data.push_back(FrontFace);
	}

	if (render[Faces::Top]) {
		Quad TopFace;

		TopFace.v0.Position = { x - size, y + size, z + size };
		TopFace.v1.Position = { x + size, y + size, z + size };
		TopFace.v2.Position = { x + size, y + size, z - size };
		TopFace.v3.Position = { x - size, y + size, z - size };

		TopFace.v0.TC = { 0.0f, 0.0f };
		TopFace.v1.TC = { 1.0f, 0.0f };
		TopFace.v2.TC = { 1.0f, 1.0f };
		TopFace.v3.TC = { 0.0f, 1.0f };

		TopFace.v0.NV = { 0.0f, 1.0f, 0.0f };
		TopFace.v1.NV = { 0.0f, 1.0f, 0.0f };
		TopFace.v2.NV = { 0.0f, 1.0f, 0.0f };
		TopFace.v3.NV = { 0.0f, 1.0f, 0.0f };

		TopFace.v0.AtlasData = { 0.0f, 15.0f };
		TopFace.v1.AtlasData = { 0.0f, 15.0f };
		TopFace.v2.AtlasData = { 0.0f, 15.0f };
		TopFace.v3.AtlasData = { 0.0f, 15.0f };

		Data.push_back(TopFace);
	}

	if (render[Faces::Left]) {
		Quad LeftFace;

		LeftFace.v0.Position = { x - size, y - size, z - size };
		LeftFace.v1.Position = { x - size, y - size, z + size };
		LeftFace.v2.Position = { x - size, y + size, z + size };
		LeftFace.v3.Position = { x - size, y + size, z - size };

		LeftFace.v0.TC = { 0.0f, 0.0f };
		LeftFace.v1.TC = { 1.0f, 0.0f };
		LeftFace.v2.TC = { 1.0f, 1.0f };
		LeftFace.v3.TC = { 0.0f, 1.0f };

		LeftFace.v0.NV = { -1.0f, 0.0f, 0.0f };
		LeftFace.v1.NV = { -1.0f, 0.0f, 0.0f };
		LeftFace.v2.NV = { -1.0f, 0.0f, 0.0f };
		LeftFace.v3.NV = { -1.0f, 0.0f, 0.0f };

		LeftFace.v0.AtlasData = { 3.0f, 15.0f };
		LeftFace.v1.AtlasData = { 3.0f, 15.0f };
		LeftFace.v2.AtlasData = { 3.0f, 15.0f };
		LeftFace.v3.AtlasData = { 3.0f, 15.0f };

		Data.push_back(LeftFace);
	}

	if (render[Faces::Right]) {
		Quad RightFace;

		RightFace.v0.Position = { x + size, y - size, z + size };
		RightFace.v1.Position = { x + size, y - size, z - size };
		RightFace.v2.Position = { x + size, y + size, z - size };
		RightFace.v3.Position = { x + size, y + size, z + size };

		RightFace.v0.TC = { 0.0f, 0.0f };
		RightFace.v1.TC = { 1.0f, 0.0f };
		RightFace.v2.TC = { 1.0f, 1.0f };
		RightFace.v3.TC = { 0.0f, 1.0f };

		RightFace.v0.NV = { 1.0f, 0.0f, 0.0f };
		RightFace.v1.NV = { 1.0f, 0.0f, 0.0f };
		RightFace.v2.NV = { 1.0f, 0.0f, 0.0f };
		RightFace.v3.NV = { 1.0f, 0.0f, 0.0f };

		RightFace.v0.AtlasData = { 3.0f, 15.0f };
		RightFace.v1.AtlasData = { 3.0f, 15.0f };
		RightFace.v2.AtlasData = { 3.0f, 15.0f };
		RightFace.v3.AtlasData = { 3.0f, 15.0f };

		Data.push_back(RightFace);
	}

	if (render[Faces::Back]) {
		Quad BackFace;

		BackFace.v0.Position = { x + size, y - size, z - size };
		BackFace.v1.Position = { x - size, y - size, z - size };
		BackFace.v2.Position = { x - size, y + size, z - size };
		BackFace.v3.Position = { x + size, y + size, z - size };

		BackFace.v0.TC = { 0.0f, 0.0f };
		BackFace.v1.TC = { 1.0f, 0.0f };
		BackFace.v2.TC = { 1.0f, 1.0f };
		BackFace.v3.TC = { 0.0f, 1.0f };

		BackFace.v0.NV = { 0.0f, 0.0f, 1.0f };
		BackFace.v1.NV = { 0.0f, 0.0f, 1.0f };
		BackFace.v2.NV = { 0.0f, 0.0f, 1.0f };
		BackFace.v3.NV = { 0.0f, 0.0f, 1.0f };

		BackFace.v0.AtlasData = { 3.0f, 15.0f };
		BackFace.v1.AtlasData = { 3.0f, 15.0f };
		BackFace.v2.AtlasData = { 3.0f, 15.0f };
		BackFace.v3.AtlasData = { 3.0f, 15.0f };

		Data.push_back(BackFace);
	}

	if (render[Faces::Bottom]) {
		Quad BottomFace;

		BottomFace.v0.Position = { x - size, y - size, z - size };
		BottomFace.v1.Position = { x + size, y - size, z - size };
		BottomFace.v2.Position = { x + size, y - size, z + size };
		BottomFace.v3.Position = { x - size, y - size, z + size };

		BottomFace.v0.TC = { 0.0f, 0.0f };
		BottomFace.v1.TC = { 1.0f, 0.0f };
		BottomFace.v2.TC = { 1.0f, 1.0f };
		BottomFace.v3.TC = { 0.0f, 1.0f };

		BottomFace.v0.NV = { 0.0f, -1.0f, 0.0f };
		BottomFace.v1.NV = { 0.0f, -1.0f, 0.0f };
		BottomFace.v2.NV = { 0.0f, -1.0f, 0.0f };
		BottomFace.v3.NV = { 0.0f, -1.0f, 0.0f };

		BottomFace.v0.AtlasData = { 2.0f, 15.0f };
		BottomFace.v1.AtlasData = { 2.0f, 15.0f };
		BottomFace.v2.AtlasData = { 2.0f, 15.0f };
		BottomFace.v3.AtlasData = { 2.0f, 15.0f };

		Data.push_back(BottomFace);
	}

	return Data;
}

void Chunk::ClearBufferPosition(int x, int y, int z)
{
	int linearIndex = x + ChunkWidth * (y + ChunkHeight * z);
	int bufferOffset = linearIndex * bytesPerCube;

	std::vector<GLubyte> zeros(bytesPerCube, 0);
	Positions.SetBufferData(zeros.data(), bufferOffset, bytesPerCube);
}

void Chunk::SetBlockBufferData(int x, int y, int z, bool redrawNearbyBlocks)
{
	int linearIndex = x + ChunkWidth * (y + ChunkHeight * z);
	int bufferOffset = linearIndex * bytesPerCube;

	bool faces[6] = {false};
	int faceAmount = 0;

	CheckNearbyBlocks(faces, faceAmount, x, y, z);

	if (redrawNearbyBlocks) 
		RedrawNearbyBlocks(x, y, z);

	std::vector<Quad> PartialCube = GenerateCube((float)x, (float)y, (float)z, faces, faceAmount);

	Positions.SetBufferData(PartialCube.data(), bufferOffset, sizeof(Quad) * faceAmount);
}

void Chunk::SetBlock(int x, int y, int z)
{
	//Checks if its outside the chunk borders
	if (x - 1 < 0 || x > ChunkWidth) { return; }
	if (y - 1 < 0 || y > ChunkHeight) { return; }
	if (z - 1 < 0 || z > ChunkWidth) { return; }
	////////////////////////////////////////

	m_BlockMatrix[x - 1][z - 1][y - 1].setOccupation(true);
	SetBlockBufferData(x - 1, y - 1, z - 1, true);
}

void Chunk::DestroyBlock(int x, int y, int z)
{
	//Checks if its outside the chunk borders
	if (x - 1 < 0 || x > ChunkWidth) { return; }
	if (y - 1 < 0 || y > ChunkHeight) { return; }
	if (z - 1 < 0 || z > ChunkWidth) { return; }
	////////////////////////////////////////

	m_BlockMatrix[x - 1][z - 1][y - 1].setOccupation(false);
	ClearBufferPosition(x - 1, y - 1, z - 1);
	RedrawNearbyBlocks(x - 1, y - 1, z - 1);

}

void Chunk::Render() {

	C_shader.Bind();
	C_shader.SetUniformMat4f("Model", m_ModelMatrix);
	C_shader.SetUniformMat4f("View", Camera::GetViewMatrix());
	C_shader.SetUniformMat4f("Projection", Camera::GetProjection());
	C_shader.Unbind();

	Renderer Rend;
	Rend.Draw(va, C_IB, C_shader);
}
