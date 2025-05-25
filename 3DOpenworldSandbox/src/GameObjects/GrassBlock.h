#pragma once

#include "GameClasses/BufferObject.h"
#include "GameClasses/RenderObject.h"
#include "GameClasses/ShaderObject.h"

class GrassBlock
{
private:
	   RenderObject m_RenderBlock;
	   //ShaderObject m_ShaderBlock;
public:
	GrassBlock();
	~GrassBlock();
	
	void Update(float deltaTime);
};