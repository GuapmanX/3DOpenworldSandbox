#pragma once
#include <vector>
#include <optional>
#include "GameObjects/Chunk.h"

void set_chunk_buffer_size(unsigned int buffer_size);
void render_chunks();

void build_chunk(int slot, float x, float y, float z);
void clear_chunk(int slot);
