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

struct gpu_Cube {
	Quad surfaces[6];
	unsigned int faces_to_render = 0;
};

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
	glm::vec3 m_Position{ 1.0f,1.0f,1.0f };
	glm::vec3 BlocksLoaded{ 1,1,1 };

	gpu_Cube* gpu_data = new gpu_Cube[ChunkWidth * ChunkWidth * ChunkHeight];
	

	void SetBlock(int x, int y, int z);
	void DestroyBlock(int x, int y, int z);
	void Render();
	void Move(float x, float y, float z);

	Chunk(float x, float y, float z);
	//Chunk() = default;
	~Chunk();

	Chunk(const Chunk&) = default;
	Chunk& operator=(const Chunk&) = default;
	Chunk& operator=(Chunk&&) = default;

	Chunk(Chunk&& other) noexcept;

//both are seperate to make sure it's multithreadable because OpenGL hates multithreading

	//gpu data processing
	void UpdateBlock_GPU(int x, int y, int z);

	//cpu data processing
	void SetBlock_CPU(int x, int y, int z);
	void DestroyBlock_CPU(int x, int y, int z);



private:
	//general
	bool CheckForBlock(int x, int y, int z);
	void CheckNearbyBlocks(bool (&values)[6], int& faceCount,int x, int y, int z);
	//

	//universal(does CPU and GPU at the same time, but can't be multithreaded)
	void SetBlockBufferData(int x, int y, int z, bool RedrawNearbyBlocks);
	void RedrawBlock(int x, int y, int z);
	void ClearBufferPosition(int x, int y, int z);
	void RedrawNearbyBlocks(int x, int y, int z);
	void GenerateCube(std::vector<Quad>& Data, float x, float y, float z, const bool render[6], const int faces);

	//CPU only data prep that can be multithreaded
	void PrepareBlockCPUData(int x, int y, int z, bool RedrawNearbyBlocks);
	void RedrawBlock_CPU(int x, int y, int z);
	void ClearBufferPosition_CPU(int x, int y, int z);
	void RedrawNearbyBlocks_CPU(int x, int y, int z);
	void GenerateCube_CPU(Quad(&data)[6], float x, float y, float z, const bool render[6], const int faces);
};
