#pragma once
#include <vector>
#include <optional>
#include "GameObjects/Chunk.h"
#include <thread>

const int max_buffer_size = 20;
const int lazy_chunk_loading_amount = 10; //how many blocks are sent to the gpu in one frame

void set_chunk_buffer_size();
void render_chunks();

void build_chunk(int slot, float x, float y, float z);
int build_chunk_cpu(int slot);
void mt_build_chunk(int slot, float x, float y, float z);
void check_for_finished_chunks();

bool GPU_upload(int slot);

void clear_chunk(int slot);

