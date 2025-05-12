#pragma once

#include "Shader.h"
#include "functional"
#include "string"

class ShaderObject
{
private:
	std::function<void(Shader& SHADER)> m_ShaderFunction;
	std::function<void(Shader& SHADER)> m_ShaderInitFunction;
	Shader m_Shader;
	bool Created = false;
public:
	ShaderObject(std::string Destination, std::function<void(Shader& SHADER)> SF, std::function<void(Shader& SHADER)> SIF);
	~ShaderObject();

	inline void ExcecuteShader() { m_ShaderFunction(m_Shader); };
	inline void InitializeShader() { m_ShaderInitFunction(m_Shader); };
	inline Shader& GetShader() { return m_Shader; };
};