#pragma once

#include "Block_c.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shapes/Cube.h";


const int ChunkWidth = 16;
const int ChunkHeight = 32;

struct ChunkData
{
	ChunkBlock m_ChunkData[ChunkWidth][ChunkWidth][ChunkHeight];

	VertexArray m_VA;
	VertexBufferLayout m_VBL;
	//A way to access the gpu data
	VertexBuffer m_GpuChunkData = VertexBuffer(nullptr, sizeof(Cube) * ChunkWidth * ChunkWidth * ChunkHeight, GL_DYNAMIC_DRAW);

	//Populates the chunk with data
	void CompileChunk()
	{
		m_VBL.Push<float>(3);
		m_VA.AddBuffer(m_GpuChunkData,m_VBL);

	}
	



};

class Chunk
{
private:
	ChunkData Data;
public:
	Chunk() {};
	~Chunk() {};

};
