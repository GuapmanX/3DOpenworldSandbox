#include "ShaderObject.h"

ShaderObject::ShaderObject(std::string Destination, std::function<void(Shader& SHADER)> SF, std::function<void(Shader& SHADER)> SIF)
	:m_ShaderFunction(SF), m_ShaderInitFunction(SIF)
{
	m_Shader = Shader(Destination);
}

ShaderObject::~ShaderObject()
{
}
