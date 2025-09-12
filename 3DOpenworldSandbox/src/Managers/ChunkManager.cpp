#include "ChunkManager.h"
#include <iostream>
#include <future>
#include <optional>
#include <list>

struct ChunkConstructionData
{
	int slot;
	float x, y, z;
};

std::vector<std::optional<Chunk>> ChunkCluster;
std::mutex* mutexes = new std::mutex[max_buffer_size];


std::vector<std::optional<std::shared_future<int>>> Tasks;

std::list<ChunkConstructionData> chunk_queue;



void set_chunk_buffer_size()
{
	Tasks.resize(max_buffer_size);
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

int build_chunk_cpu(int slot) {
	if (slot > max_buffer_size or slot < 0) {
		std::cout << "Buffer size exceeded!" << "\n";
		return -1;
	}

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

	return slot;
}

bool GPU_upload(int slot) {
	if (slot > max_buffer_size or slot < 0) {
		std::cout << "Buffer size exceeded!" << "\n";
		return false;
	}

	unsigned int blocks_built = 0;
	glm::vec3 Loaded = ChunkCluster[slot].value().BlocksLoaded;

	for (int x = Loaded.x; x < ChunkWidth + 1; x++)
	{
		Loaded.x = 1;
		for (int y = Loaded.y; y < ChunkHeight + 1; y++)
		{
			Loaded.y = 1;
			for (int z = Loaded.z; z < ChunkWidth + 1; z++)
			{
				Loaded.z = 1;
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
	
	ChunkCluster[slot].reset();
}

void render_chunks() {
	for (unsigned int i = 0; i < ChunkCluster.capacity(); i++) {
		if (mutexes[i].try_lock()) {
			if (ChunkCluster[i].has_value()) {
				ChunkCluster[i].value().Render();
				//std::cout << i << "\n";
			}
			else {
				//std::cout << "failed on index:" << i << std::endl;
			}
			mutexes[i].unlock();
		}
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
					ChunkCluster[i].value().loaded = true;//allows it to be rendered
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
	append_task(std::async(std::launch::async, build_chunk_cpu, slot));
}

void queue_chunk_build(int slot, float c_x, float c_y, float c_z) {
	chunk_queue.push_front({ slot,c_x,c_y,c_z });
}