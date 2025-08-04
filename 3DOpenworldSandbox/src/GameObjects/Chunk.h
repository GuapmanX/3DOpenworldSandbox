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


void Initialize();

struct BlockData
{
	int type = 0;
	bool full = false;

	const bool isEmpty() {
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
	VertexBuffer Positions = VertexBuffer(nullptr, GetFullCubeSize() * ChunkWidth * ChunkWidth * ChunkHeight, GL_DYNAMIC_DRAW);
	VertexArray va;

	void SetBlock(int x, int y, int z);
	void Render();
	Chunk();
private:
	bool CheckForBlock(int x, int y, int z);
	void CheckNearbyBlocks(bool (&values)[6], int& faces,int x, int y, int z);
	void SetBlockBufferData(int x, int y, int z, bool RedrawNearbyBlocks);
	void RedrawBlock(int x, int y, int z);
	void ClearBufferPosition(int x, int y, int z);
	void RedrawNearbyBlocks(int x, int y, int z);
};
