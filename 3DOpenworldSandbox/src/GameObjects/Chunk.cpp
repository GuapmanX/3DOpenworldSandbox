#include "Chunk.h"
#include "GameCore/Camera.h"

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
	if (x < 0 || x > ChunkWidth) { return false; }
	if (y < 0 || y > ChunkHeight) { return false; }
	if (z < 0 || z > ChunkWidth) { return false; }
	//////////////////////////////////////////


	return m_BlockMatrix[x][z][y].isEmpty(); //Checks if theres a block in the specific space
}

void Chunk::CheckNearbyBlocks(bool (&values)[6], int x, int y, int z)
{
	/*bool up = CheckForBlock(x, y + 1, z);
	bool down = CheckForBlock(x, y - 1, z);
	bool right = CheckForBlock(x + 1, y, z);
	bool left = CheckForBlock(x - 1, y, z);
	bool front = CheckForBlock(x, y, z - 1);
	bool back = CheckForBlock(x, y, z + 1);*/


	//kind of works but face direction is completely fucked
	values[Faces::Top] = CheckForBlock(x, y + 1, z);
	values[Faces::Bottom] = CheckForBlock(x, y - 1, z);
	values[Faces::Right] = CheckForBlock(x + 1, y, z);
	values[Faces::Left] = CheckForBlock(x - 1, y, z);
	values[Faces::Front] = CheckForBlock(x, y, z - 1);
	values[Faces::Back] = CheckForBlock(x, y, z + 1);

}

void Chunk::SetBlockBufferData(int x, int y, int z)
{
	int bytesPerCube = GetFullCubeSize();
	int linearIndex = x + ChunkWidth * (y + ChunkHeight * z);
	int bufferOffset = linearIndex * bytesPerCube;

	bool faces[6] = {true};// = { true, true, true, true, true, true };

	CheckNearbyBlocks(faces, x, y, z);

	//Cube<6> CubeForChunk = CreateCube<6>(0.0f, 0.0f, 0.0f, 0.5f, faces);
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
	SetBlockBufferData(x, y, z);
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
