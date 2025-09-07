#include "ChunkManager.h"
#include <iostream>
#include <future>



const int max_buffer_size = 20;

std::vector<std::optional<Chunk>> ChunkCluster;
std::mutex mutexes[max_buffer_size];

std::vector<std::future<void>> Tasks;

void set_chunk_buffer_size()
{
	ChunkCluster.resize(max_buffer_size);
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

void build_chunk_cpu(int slot, float c_x, float c_y, float c_z) {
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
				ChunkCluster[slot].value().SetBlock_CPU(x, y, z);
			}
		}
	}
}

void GPU_upload(int slot) {
	if (slot > max_buffer_size or slot < 0) {
		std::cout << "Buffer size exceeded!" << "\n";
		return;
	}


	for (int x = 1; x < ChunkWidth + 1; x++)
	{
		for (int y = 1; y < ChunkHeight + 1; y++)
		{
			for (int z = 1; z < ChunkWidth + 1; z++)
			{
				ChunkCluster[slot].value().UpdateBlock_GPU(x, y, z);
			}
		}
	}
}

void clear_chunk(int slot) {
	
	ChunkCluster[slot].reset();
}

void render_chunks() {
	for (unsigned int i = 0; i < ChunkCluster.capacity(); i++) {
		if (mutexes[i].try_lock()) {
			if (ChunkCluster[i].has_value()) {
				ChunkCluster[i].value().Render();
				//std::cout << i << "\n";
			}
			mutexes[i].unlock();
		}
		else
		{
			//std::cout << "failed on index: " << i << std::endl;
		}
	}
}


void build_chunk_formt(unsigned int index) {
	std::lock_guard<std::mutex> lock(mutexes[index]);

	for (int x = 1; x < ChunkWidth + 1; x++)
	{
		for (int y = 1; y < ChunkHeight + 1; y++)
		{
			for (int z = 1; z < ChunkWidth + 1; z++)
			{
				ChunkCluster[index].value().SetBlock(x, y, z);
				std::cout << "not freezing" << std::endl;
			}
		}
	}
}

void mt_build_chunk(int slot, float c_x, float c_y, float c_z) {
	if (slot > max_buffer_size or slot < 0) {
		std::cout << "Buffer size exceeded!" << "\n";
		return;
	}
	ChunkCluster[slot] = Chunk(c_x, c_y, c_z);
	Tasks.push_back(std::async(std::launch::async, build_chunk_formt, slot));
}