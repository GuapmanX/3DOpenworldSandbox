#include "Chunk.h"
#include "GameCore/Camera.h"
#include <iostream>

const int bytesPerCube = GetFullCubeSize();
unsigned int* Indices = new unsigned int[ChunkWidth * ChunkWidth * ChunkHeight * 36];
VertexBufferLayout C_VBL;
IndexBuffer C_IB;
Shader C_shader;
Texture C_Texture;
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
	C_VBL.Push<float>(2); //atlas size
	C_VBL.Push<float>(2); //single texture size
	C_VBL.Push<float>(2); //texture position
	////////
}

void SetTexture()
{
	C_Texture = Texture("res/textures/terrain.png");
}

void Initialize() {
	SetIndexBuffer();
	SetShader();
	Setlayout();
	SetTexture();
}

Chunk::Chunk()
{
	va.AddBuffer(Positions, C_VBL);
	C_shader.Bind();
	C_Texture.Bind(0);
	C_shader.SetUniform1i("u_Texture", 0);
	C_shader.Unbind();
}


bool Chunk::CheckForBlock(int x, int y, int z)
{
	//Checks if its outside the chunk borders
	if (x < 0 || x > ChunkWidth) { return true; }
	if (y < 0 || y > ChunkHeight) { return true; }
	if (z < 0 || z > ChunkWidth) { return true; }
	//////////////////////////////////////////


	return m_BlockMatrix[x][z][y].isEmpty(); //Checks if theres a block in the specific space
}

void Chunk::CheckNearbyBlocks(bool (&values)[6],int& faces, int x, int y, int z)
{
	bool IsTop = CheckForBlock(x, y + 1, z);
	faces += IsTop;
	values[Faces::Top] = IsTop;

	bool IsBottom = CheckForBlock(x, y - 1, z);
	faces += IsBottom;
	values[Faces::Bottom] = IsBottom;

	bool IsRight = CheckForBlock(x + 1, y, z);
	faces += IsRight;
	values[Faces::Right] = IsRight;

	bool IsLeft = CheckForBlock(x - 1, y, z);
	faces += IsLeft;
	values[Faces::Left] = IsLeft;

	bool IsFront = CheckForBlock(x, y, z + 1);
	faces += IsFront;
	values[Faces::Front] = IsFront;

	bool IsBack = CheckForBlock(x, y, z - 1);
	faces += IsBack;
	values[Faces::Back] = IsBack;
}

void Chunk::ClearBufferPosition(int x, int y, int z)
{
	int linearIndex = x + ChunkWidth * (y + ChunkHeight * z);
	int bufferOffset = linearIndex * bytesPerCube;

	Positions.SetBufferData(0, bufferOffset, bytesPerCube);
}

void Chunk::RedrawBlock(int x, int y, int z)
{
	//Checks if its outside the chunk borders
	if (x < 0 || x > ChunkWidth) { return; }
	if (y < 0 || y > ChunkHeight) { return; }
	if (z < 0 || z > ChunkWidth) { return; }
	//////////////////////////////////////////

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

void Chunk::SetBlockBufferData(int x, int y, int z, bool redrawNearbyBlocks)
{
	int linearIndex = x + ChunkWidth * (y + ChunkHeight * z);
	int bufferOffset = linearIndex * bytesPerCube;

	bool faces[6] = {true};
	int faceAmount = 0;

	CheckNearbyBlocks(faces, faceAmount, x, y, z);
	std::cout << faceAmount << std::endl;

	if (redrawNearbyBlocks) 
		RedrawNearbyBlocks(x, y, z);

	Cube<6> CubeForChunk = CreateCube<6>((float)x, (float)y, (float)z, 0.5f, faces);

	CubeForChunk.SetQuadTextureData(Faces::Front, { 256.0f, 256.0f, 16.0f, 16.0f, 3.0f, 15.0f });
	CubeForChunk.SetQuadTextureData(Faces::Top, { 256.0f, 256.0f, 16.0f, 16.0f, 0.0f, 15.0f });
	CubeForChunk.SetQuadTextureData(Faces::Left, { 256.0f, 256.0f, 16.0f, 16.0f, 3.0f, 15.0f });
	CubeForChunk.SetQuadTextureData(Faces::Right, { 256.0f, 256.0f, 16.0f, 16.0f, 3.0f, 15.0f });
	CubeForChunk.SetQuadTextureData(Faces::Back, { 256.0f, 256.0f, 16.0f, 16.0f, 3.0f, 15.0f });
	CubeForChunk.SetQuadTextureData(Faces::Bottom, { 256.0f, 256.0f, 16.0f, 16.0f, 2.0f, 15.0f });

	Positions.SetBufferData(&CubeForChunk.Quads, bufferOffset, CubeForChunk.GetBufferSize());
}

void Chunk::SetBlock(int x, int y, int z)
{
	m_BlockMatrix[x][z][y].setOccupation(true);
	SetBlockBufferData(x, y, z, true);
}

void Chunk::Render() {

	C_shader.Bind();
	C_shader.SetUniformMat4f("Model", glm::mat4(1.0f));
	C_shader.SetUniformMat4f("View", Camera::GetViewMatrix());
	C_shader.SetUniformMat4f("Projection", Camera::GetProjection());
	C_shader.Unbind();

	Renderer Rend;
	Rend.Draw(va, C_IB, C_shader);
}
