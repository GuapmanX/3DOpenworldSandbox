#pragma once

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Shapes/VertexStructs.h"
#include "CubeMap.h"
#include <array>
#include <vector>
#include <string>

#include "Texture.h"

struct SkyboxVertex{
	Vector3 Position{ 0.0f,0.0f,0.0f };
	TextureCoordinates2D TC{ 0.0f,0.0f };
	AtlasData AtlasData{ 0.0f, 0.0f };
};

struct SkyboxQuad {
	SkyboxVertex v0, v1, v2, v3;
};

class Skybox
{
private:
	VertexBuffer VB;
	VertexArray VA;
	Shader SH;
	IndexBuffer IB;
	VertexBufferLayout VBL;
	Texture SkyTex = Texture("res/textures/skybox.png",GL_LINEAR);
	std::vector<SkyboxQuad> GenerateCube(float x, float y, float z);
public:
	Skybox();
	void Render();
};