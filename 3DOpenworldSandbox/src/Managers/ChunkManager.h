#pragma once
#include <vector>
#include <optional>
#include "GameObjects/Chunk.h"
#include <thread>

void set_chunk_buffer_size();
void render_chunks();

void build_chunk(int slot, float x, float y, float z);
void build_chunk_cpu(int slot, float x, float y, float z);
void mt_build_chunk(int slot, float x, float y, float z);

void GPU_upload(int slot);

void clear_chunk(int slot);

