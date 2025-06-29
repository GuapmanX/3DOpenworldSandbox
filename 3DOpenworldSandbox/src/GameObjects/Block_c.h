#pragma once
/*These blocks are exclusively designed to work with chunks, the ones without the _c in their name
are the only ones that work independent of chunks*/
#include <string>
#include "CubeMap.h"

struct ChunkBlock
{
	bool Empty = false;
	CubeMap Texture;
	std::string BlockName;

};