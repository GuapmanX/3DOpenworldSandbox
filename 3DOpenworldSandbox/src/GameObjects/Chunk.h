#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
//#include "Shapes/Cube.h"
#include "Shapes/dynamicCube.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>


const int ChunkWidth = 16;
const int ChunkHeight = 32;
const int bytesPerCube = sizeof(Quad) * 6;


void Initialize();

struct BlockData
{
	int type = 0;
	bool full = false;

	const bool isEmpty() const {
		return !full;
	}

	void setOccupation(const bool& occupied) {
		full = occupied;
	}
};

class Chunk
{
public:
	BlockData m_BlockMatrix[ChunkWidth][ChunkWidth][ChunkHeight]; // XZY
	VertexBuffer Positions{ nullptr, bytesPerCube * (ChunkWidth) * (ChunkWidth) * (ChunkHeight), GL_DYNAMIC_DRAW };
	VertexArray va;

	glm::mat4 m_ModelMatrix{ 1.0f };
	glm::vec3 m_Position{ 0.0f,0.0f,0.0f };

	void SetBlock(int x, int y, int z);
	void DestroyBlock(int x, int y, int z);
	void Render();
	void Move(float x, float y, float z);

	Chunk(float x, float y, float z);
	~Chunk();

	Chunk& operator=(Chunk&& other) noexcept;
	Chunk& operator=(Chunk& other) noexcept;
private:
	bool CheckForBlock(int x, int y, int z);
	void CheckNearbyBlocks(bool (&values)[6], int& faceCount,int x, int y, int z);
	void SetBlockBufferData(int x, int y, int z, bool RedrawNearbyBlocks);
	void RedrawBlock(int x, int y, int z);
	void ClearBufferPosition(int x, int y, int z);
	void RedrawNearbyBlocks(int x, int y, int z);
	std::vector<Quad> GenerateCube(float x, float y, float z, const bool render[6], const int faces);
};
