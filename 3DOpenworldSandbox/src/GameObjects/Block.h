#pragma once

#include "GameClasses/BufferObject.h"
#include "GameClasses/RenderObject.h"
#include "GameClasses/ShaderObject.h"

void InitBuffer();

class Block
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

	Block(const glm::vec3& Position = glm::vec3(0.0f, 0.0f, 0.0f));
	~Block();
	
	void Update(const float& deltaTime);
	void ChangeTexture(const std::array<std::string, 6>& FACES);

	void ApplyLighting(const glm::vec4& SourceColor, glm::vec3& SourcePosition);
};