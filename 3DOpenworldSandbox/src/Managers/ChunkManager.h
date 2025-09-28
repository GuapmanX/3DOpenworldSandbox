#pragma once
#include "GameObjects/Chunk.h"

const int max_buffer_size = 110;
const int lazy_chunk_loading_amount = 1000; //how many blocks are sent to the gpu in one frame
const int lazy_chunk_construction_amount = 1; //how many chunks can be constructed in one frame

void set_chunk_buffer_size();
void render_chunks();

//void build_chunk(int slot, float x, float y, float z);
int build_chunk_cpu(int slot);
void mt_build_chunk(int slot, float x, float y, float z);
void check_for_finished_chunks();
void destroy_block(int x, int y, int z);

void move_build_queue();
void queue_chunk_build(int slot, float c_x, float c_y, float c_z);

bool GPU_upload(int slot);

void clear_chunk(int slot);

struct Key2
{
    int x, y;

    Key2(int _x, int _y)
        :x(_x), y(_y)
    {
    }

    //the only necessary operator overload for std::map
    bool operator<(const Key2& other)const {
        if (x < other.x) return true;
        if (x > other.x) return false;
        return y < other.y;
    }

    bool operator()(const Key2& a, const Key2& b)const
    {
        return a.x == b.x && a.y == b.y;
    }
};

