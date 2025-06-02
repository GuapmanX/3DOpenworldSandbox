#pragma once

#include "BufferObject.h"
#include "ShaderObject.h"

#include "RendererClasses/CubeMap.h"
#include <array>

class RenderObject
{
private:
	BufferObject* m_Buffer;
	Shader* m_Shader;
public:
	RenderObject() = default;
	RenderObject(BufferObject* p_Buffer, Shader* p_Shader);
	~RenderObject();
	void Draw(float DeltaTime);
	inline void addBufferObject(BufferObject* p_Buffer) { m_Buffer = p_Buffer; }
	inline void addShaderObject(Shader* p_Shader) { m_Shader = p_Shader; }
	inline Shader& GetShader() { return *m_Shader; }
};
