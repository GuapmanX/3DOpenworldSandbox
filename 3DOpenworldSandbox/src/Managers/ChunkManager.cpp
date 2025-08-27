#include "ChunkManager.h"
#include <iostream>

std::vector<std::optional<Chunk>> ChunkCluster;
int max_buffer_size = 0;

void set_chunk_buffer_size(unsigned int buffer_size)
{
	max_buffer_size = buffer_size;
	ChunkCluster.resize(buffer_size);
}

void build_chunk(int slot, float c_x, float c_y, float c_z) {
	if (slot > max_buffer_size or slot < 0) { 
		std::cout << "Buffer size exceeded!" << "\n";
		return; 
	}

	ChunkCluster[slot] = Chunk(c_x, c_y, c_z);

	for (int x = 1; x < ChunkWidth + 1; x++)
	{
		for (int y = 1; y < ChunkHeight + 1; y++)
		{
			for (int z = 1; z < ChunkWidth + 1; z++)
			{
				ChunkCluster[slot].value().SetBlock(x, y, z);
			}
		}
	}
}

void clear_chunk(int slot) {
	ChunkCluster[slot].reset();
}

void render_chunks() {
	for (unsigned int i = 0; i < ChunkCluster.capacity(); i++) {
		if (ChunkCluster[i].has_value()) {
			ChunkCluster[i].value().Render();
			//std::cout << i << "\n";
		}
	}
}