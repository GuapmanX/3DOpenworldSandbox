#pragma once

#include "GameClasses/BufferObject.h"
#include "GameClasses/RenderObject.h"
#include "GameClasses/ShaderObject.h"

void InitLightBuffer();

class LightBlock
{
private:
		/* for rendering only */
	   RenderObject m_RenderBlock;
	   CubeMap m_cubeMap;
	   ////////////////////////

	   glm::mat4 m_ModelMatrix;
public:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec4 m_Color;

	LightBlock(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f));
	~LightBlock();
	
	void Update(const float& deltaTime);
	void ChangeTexture(const std::array<std::string, 6>& FACES);
	void SetLightColor(const glm::vec4& Color);
	inline glm::vec4& GetColor() { return m_Color; }
};