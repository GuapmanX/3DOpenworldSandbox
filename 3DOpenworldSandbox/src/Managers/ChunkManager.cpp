#include "ChunkManager.h"
#include <iostream>

std::vector<std::optional<Chunk>> ChunkCluster;
int max_buffer_size = 0;

void set_chunk_buffer_size(unsigned int buffer_size)
{
	max_buffer_size = buffer_size;
	ChunkCluster.reserve(buffer_size);

	for (unsigned int i = 0; i < ChunkCluster.capacity(); i++) {
		std::optional<Chunk> empty_value;
		ChunkCluster.push_back(empty_value); //fills buffer with empty optional values
	}

	ChunkCluster[5] = Chunk(0.0f, 0.0f, 0.0f);
	for (int x = 1; x < ChunkWidth + 1; x++)
	{
		for (int y = 1; y < ChunkHeight + 1; y++)
		{
			for (int z = 1; z < ChunkWidth + 1; z++)
			{
				ChunkCluster[5].value().SetBlock(x, y, z);
			}
		}
	}
}

void render_chunks() {
	for (unsigned int i = 0; i < ChunkCluster.capacity(); i++) {
		if (ChunkCluster[i].has_value()) {
			ChunkCluster[i].value().Render();
		}
	}
}