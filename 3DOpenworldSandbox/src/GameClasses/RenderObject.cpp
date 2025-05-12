#include "RenderObject.h"
#include "GameCore/Camera.h"
#include <iostream>



RenderObject::RenderObject(BufferObject* p_Buffer, ShaderObject* p_Shader)
	:m_Buffer(p_Buffer), m_Shader(p_Shader)
{
    Shader& shader = m_Shader->GetShader();

    shader.Bind();
    CM.Bind(0);
    shader.SetUniform1i("u_Texture", 0);
	shader.Unbind();

}

RenderObject::~RenderObject()
{
}

void RenderObject::Draw(float DeltaTime)
{
	if ((m_Buffer) && (m_Shader))
	{
		//Shader shader = *m_Shader;
		Shader& shader = m_Shader->GetShader();

		glm::mat4 Model(1.0f);
		glm::mat4 View = Camera::GetViewMatrix();
		glm::mat4 Projection = Camera::GetProjection();

		glm::mat4 mvp = Projection * View * Model;



		Renderer DRAWER;

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvp);
		DRAWER.Draw(m_Buffer->GetVA(), m_Buffer->GetIB(), shader);
	}
}
