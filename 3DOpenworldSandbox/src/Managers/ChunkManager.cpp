#include "ChunkManager.h"
#include <iostream>
#include <future>
#include <optional>
#include <list>
#include <map>
#include <cmath>





struct ChunkConstructionData
{
	int slot;
	float x, y, z;
};

std::vector<std::optional<Chunk>> ChunkCluster;
std::mutex* mutexes = new std::mutex[max_buffer_size];


std::vector<std::optional<std::shared_future<int>>> Tasks;
std::map<Key2, unsigned int> position_index; //gives the index of a chunk based on it's positions

std::list<ChunkConstructionData> chunk_queue;



void set_chunk_buffer_size()
{
	Tasks.resize(max_buffer_size);
	ChunkCluster.resize(max_buffer_size);
}

//void build_chunk(int slot, float c_x, float c_y, float c_z) {
//	if (slot > max_buffer_size or slot < 0) { 
//		std::cout << "Buffer size exceeded!" << "\n";
//		return; 
//	}
//
//	ChunkCluster[slot] = Chunk(c_x, c_y, c_z);
//
//	for (int x = 1; x < ChunkWidth + 1; x++)
//	{
//		for (int y = 1; y < ChunkHeight + 1; y++)
//		{
//			for (int z = 1; z < ChunkWidth + 1; z++)
//			{
//				ChunkCluster[slot].value().SetBlock(x, y, z);
//			}
//		}
//	}
//}

int build_chunk_cpu(int slot) {
	if (slot > max_buffer_size or slot < 0) {
		std::cout << "Buffer size exceeded!" << "\n";
		return -1;
	}

	for (int x = 0; x < ChunkWidth; x++)
	{
		for (int y = 0; y < ChunkHeight/2; y++)
		{
			for (int z = 0; z < ChunkWidth; z++)
			{
				ChunkCluster[slot].value().SetBlock_CPU(x, y, z);
			}
		}
	}

	return slot;
}

bool GPU_upload(int slot) {
	if (slot > max_buffer_size or slot < 0) {
		std::cout << "Buffer size exceeded!" << "\n";
		return false;
	}

	unsigned int blocks_built = 0;
	glm::ivec3 Loaded = ChunkCluster[slot].value().BlocksLoaded;

	for (int x = Loaded.x; x < ChunkWidth; x++)
	{
		Loaded.x = 0;
		for (int y = Loaded.y; y < ChunkHeight; y++)
		{
			Loaded.y = 0;
			for (int z = Loaded.z; z < ChunkWidth; z++)
			{
				Loaded.z = 0;
				//std::cout << x << " " << y << " " << z << std::endl;
				ChunkCluster[slot].value().UpdateBlock_GPU(x, y, z);
				blocks_built++;
				if (blocks_built >= lazy_chunk_loading_amount) {
					ChunkCluster[slot].value().BlocksLoaded = glm::vec3(x, y, z);
					return false;
				}
			}
		}
	}
	return true;
}

void clear_chunk(int slot) {
	
	int x = ChunkCluster[slot].value().location_index.x;
	int z = ChunkCluster[slot].value().location_index.y;
	position_index.erase({ x, z });
	ChunkCluster[slot].reset();
}

void render_chunks() {
	for (unsigned int i = 0; i < ChunkCluster.capacity(); i++) {
		//if (mutexes[i].try_lock()) { //realised that mutexes are useless in this implementation
			if (ChunkCluster[i].has_value()) {
				ChunkCluster[i].value().Render();
				//std::cout << i << "\n";
			}
			//else {
				//std::cout << "failed on index:" << i << std::endl;
			//}
			//mutexes[i].unlock();
		//}
	}
}

void check_for_finished_chunks() {
	for (unsigned int i = 0; i < Tasks.capacity(); i++) {
		if (Tasks[i].has_value()) {
			if (Tasks[i].value().valid()) {
				//identify which exact chunk to build in a memory safe manner
				int chunk_index = Tasks[i].value().get();
				bool finished = GPU_upload(chunk_index);
				//std::cout << finished << std::endl;
				if (finished) {
					Tasks[i] = std::shared_future<int>();
					Tasks[i].reset();
					ChunkCluster[chunk_index].value().loaded = true;//allows it to be rendered
					ChunkCluster[chunk_index].value().clear_cpu_buffer();
				}
				else
				{
					break;
				}
			}
		}
	}
}

void move_build_queue() {
	for (unsigned int i = 0; i < lazy_chunk_construction_amount; i++) {
		if (chunk_queue.empty()) { break; }
		ChunkConstructionData Data = chunk_queue.back();
		mt_build_chunk(Data.slot, Data.x, Data.y, Data.z);
		chunk_queue.pop_back();
	}
}

static void append_task(std::shared_future<int> method) {
	for (unsigned int i = 0; i < Tasks.capacity(); i++) {
		if (!Tasks[i].has_value()) {
			Tasks[i] = std::move(method);
			break;
		}
	}
}

void mt_build_chunk(int slot, float c_x, float c_y, float c_z) {
	if (slot > max_buffer_size or slot < 0) {
		std::cout << "Buffer size exceeded!" << "\n";
		return;
	}

	ChunkCluster[slot] = Chunk(c_x, c_y, c_z);
	std::cout << ChunkCluster.capacity() << std::endl;
	std::cout << slot << std::endl;

	int offset_x = std::floorf((c_x + f_ChunkWidth / 2.0f) / f_ChunkWidth);
	int offset_z = std::floorf((c_z + f_ChunkWidth / 2.0f) / f_ChunkWidth);
	//std::cout << "Chunk " << slot << "has a offset of " << offset_x << " " << offset_z << std::endl;

	ChunkCluster[slot].value().location_index = { offset_x, offset_z };
	position_index[{offset_x, offset_z}] = slot;
	int x2 = c_x / ChunkWidth;
	int z2 = c_z / ChunkWidth;

	//std::cout << " " << std::endl;
	//std::cout << "fake offset is: " << x2 << " " << z2 << "\n";
	//std::cout << "offset is: " << offset_x << " " << offset_z << "\n";
	//std::cout << "slot is: " << slot << std::endl;
	//std::cout << "sanity check:" << position_index[Key2(x2, z2)] << std::endl;

	append_task(std::async(std::launch::async, build_chunk_cpu, slot));
}

void queue_chunk_build(int slot, float c_x, float c_y, float c_z) {
	chunk_queue.push_front({ slot,c_x,c_y,c_z });
}

void destroy_block(int x, int y, int z)
{
	int offset_x = x / ChunkWidth;
	int offset_z = z / ChunkWidth;

	//gets the chunk in which the block is located in
	int chunkIndex = position_index[{offset_x, offset_z}];
	
	int blockpos_x = x % ChunkWidth;
	int blockpos_y = y;
	int blockpos_z = z % ChunkWidth;

	//std::cout << "SubBlock " << blockpos_x << " " << blockpos_z << std::endl;

	//std::cout << "Chunk is located at grid X:" << offset_x << " Y: " << offset_z << "\n";
	//std::cout << "index is: " << chunkIndex << "\n";

	ChunkCluster[chunkIndex].value().DestroyBlock(blockpos_x,blockpos_y,blockpos_z);
}