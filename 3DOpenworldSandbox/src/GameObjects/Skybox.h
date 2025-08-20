#pragma once

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Shapes/VertexStructs.h"
#include <vector>

#include "Texture.h"

class Skybox
{
private:
	VertexBuffer VB;
	VertexArray VA;
	Shader SH;
	IndexBuffer IB;
	VertexBufferLayout VBL;
	Texture SkyTex = Texture("res/textures/terrain.png");
	std::vector<Quad> GenerateCube(float x, float y, float z);
public:
	Skybox();
	void Render();
};