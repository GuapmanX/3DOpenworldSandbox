#pragma once

#include "BufferObject.h"
#include "ShaderObject.h"

#include "RendererClasses/CubeMap.h"
#include <array>

/*std::array<std::string, 6> texturesCB = {
        "res/textures/GrassBlockSide.png", //RIGHT
        "res/textures/GrassBlockSide.png", //LEFT
        "res/textures/GrassBlockTop.png",//TOP
        "res/textures/GrassBlockBottom.png", //BOTTOM
        "res/textures/GrassBlockSide.png", //FRONT
        "res/textures/GrassBlockSide.png", //REAR
};*/

class RenderObject
{
private:
    std::array<std::string, 6> texturesCB = {
        "res/textures/GrassBlockSide.png", //RIGHT
        "res/textures/GrassBlockSide.png", //LEFT
        "res/textures/GrassBlockTop.png",//TOP
        "res/textures/GrassBlockBottom.png", //BOTTOM
        "res/textures/GrassBlockSide.png", //FRONT
        "res/textures/GrassBlockSide.png", //REAR
    };

	BufferObject* m_Buffer;
	ShaderObject* m_Shader;
    CubeMap CM = CubeMap(texturesCB);

public:
	RenderObject(BufferObject* Buffer, ShaderObject* p_Shader);
	~RenderObject();
	void Draw(float DeltaTime);
};
