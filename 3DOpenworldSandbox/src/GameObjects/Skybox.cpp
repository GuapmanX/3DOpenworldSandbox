#include "Skybox.h"
#include "Shapes/enums.h"
#include "GameCore/Camera.h"

Skybox::Skybox()
{
	std::vector<SkyboxQuad> Vertices = GenerateCube(0.0f, 0.0f, 0.0f);
	VB = VertexBuffer(Vertices.data(), sizeof(SkyboxQuad) * 6);

	VBL.Push<float>(3);
	VBL.Push<float>(2);
	VBL.Push<float>(2);




	VA.AddBuffer(VB, VBL);

	unsigned int Indices[36];

	for (int face = 0; face < 6; face++)
	{
		Indices[0 + face * 6] = 2 + face * 4;
		Indices[1 + face * 6] = 1 + face * 4;
		Indices[2 + face * 6] = 0 + face * 4;
		Indices[3 + face * 6] = 0 + face * 4;
		Indices[4 + face * 6] = 3 + face * 4;
		Indices[5 + face * 6] = 2 + face * 4;
	}

	IB = IndexBuffer(Indices, 36);

	SkyTex.Bind(1);
	SH = Shader("res/shaders/Skybox.shader");
	SH.Bind();
	SH.SetUniform1i("u_Texture", 1);
	SH.Unbind();
}

void Skybox::Render()
{
	SH.Bind();
	SH.SetUniformMat4f("Model", glm::translate(glm::mat4(1.0f), Camera::GetPosition()));
	SH.SetUniformMat4f("View", Camera::GetViewMatrix());
	SH.SetUniformMat4f("Projection", Camera::GetProjection());
	SH.Unbind();

	Renderer Rend;
	Rend.Draw(VA, IB, SH);
}

std::vector<SkyboxQuad> Skybox::GenerateCube(float x, float y, float z)
{
	std::vector<SkyboxQuad> Data;
	Data.reserve(6);

	const float size = 10000.0f;

		SkyboxQuad FrontFace;

		FrontFace.v0.Position = { x - size, y - size, z + size };
		FrontFace.v1.Position = { x + size, y - size, z + size };
		FrontFace.v2.Position = { x + size, y + size, z + size };
		FrontFace.v3.Position = { x - size, y + size, z + size };

		FrontFace.v0.TC = { 0.0f, 0.0f };
		FrontFace.v1.TC = { 1.0f, 0.0f };
		FrontFace.v2.TC = { 1.0f, 1.0f };
		FrontFace.v3.TC = { 0.0f, 1.0f };

		FrontFace.v0.AtlasData = { 1.0f, 1.0f };
		FrontFace.v1.AtlasData = { 1.0f, 1.0f };
		FrontFace.v2.AtlasData = { 1.0f, 1.0f };
		FrontFace.v3.AtlasData = { 1.0f, 1.0f };

		Data.push_back(FrontFace);


		SkyboxQuad TopFace;

		TopFace.v0.Position = { x - size, y + size, z + size };
		TopFace.v1.Position = { x + size, y + size, z + size };
		TopFace.v2.Position = { x + size, y + size, z - size };
		TopFace.v3.Position = { x - size, y + size, z - size };

		TopFace.v0.TC = { 0.0f, 0.0f };
		TopFace.v1.TC = { 1.0f, 0.0f };
		TopFace.v2.TC = { 1.0f, 1.0f };
		TopFace.v3.TC = { 0.0f, 1.0f };

		TopFace.v0.AtlasData = { 1.0f, 2.0f };
		TopFace.v1.AtlasData = { 1.0f, 2.0f };
		TopFace.v2.AtlasData = { 1.0f, 2.0f };
		TopFace.v3.AtlasData = { 1.0f, 2.0f };

		Data.push_back(TopFace);


		SkyboxQuad LeftFace;

		LeftFace.v0.Position = { x - size, y - size, z - size };
		LeftFace.v1.Position = { x - size, y - size, z + size };
		LeftFace.v2.Position = { x - size, y + size, z + size };
		LeftFace.v3.Position = { x - size, y + size, z - size };

		LeftFace.v0.TC = { 0.0f, 0.0f };
		LeftFace.v1.TC = { 1.0f, 0.0f };
		LeftFace.v2.TC = { 1.0f, 1.0f };
		LeftFace.v3.TC = { 0.0f, 1.0f };

		LeftFace.v0.AtlasData = { 0.0f, 1.0f };
		LeftFace.v1.AtlasData = { 0.0f, 1.0f };
		LeftFace.v2.AtlasData = { 0.0f, 1.0f };
		LeftFace.v3.AtlasData = { 0.0f, 1.0f };

		Data.push_back(LeftFace);


		SkyboxQuad RightFace;

		RightFace.v0.Position = { x + size, y - size, z + size };
		RightFace.v1.Position = { x + size, y - size, z - size };
		RightFace.v2.Position = { x + size, y + size, z - size };
		RightFace.v3.Position = { x + size, y + size, z + size };

		RightFace.v0.TC = { 0.0f, 0.0f };
		RightFace.v1.TC = { 1.0f, 0.0f };
		RightFace.v2.TC = { 1.0f, 1.0f };
		RightFace.v3.TC = { 0.0f, 1.0f };

		RightFace.v0.AtlasData = { 2.0f, 1.0f };
		RightFace.v1.AtlasData = { 2.0f, 1.0f };
		RightFace.v2.AtlasData = { 2.0f, 1.0f };
		RightFace.v3.AtlasData = { 2.0f, 1.0f };

		Data.push_back(RightFace);


		SkyboxQuad BackFace;

		BackFace.v0.Position = { x + size, y - size, z - size };
		BackFace.v1.Position = { x - size, y - size, z - size };
		BackFace.v2.Position = { x - size, y + size, z - size };
		BackFace.v3.Position = { x + size, y + size, z - size };

		BackFace.v0.TC = { 0.0f, 0.0f };
		BackFace.v1.TC = { 1.0f, 0.0f };
		BackFace.v2.TC = { 1.0f, 1.0f };
		BackFace.v3.TC = { 0.0f, 1.0f };

		BackFace.v0.AtlasData = { 3.0f, 1.0f };
		BackFace.v1.AtlasData = { 3.0f, 1.0f };
		BackFace.v2.AtlasData = { 3.0f, 1.0f };
		BackFace.v3.AtlasData = { 3.0f, 1.0f };

		Data.push_back(BackFace);


		SkyboxQuad BottomFace;

		BottomFace.v0.Position = { x - size, y - size, z - size };
		BottomFace.v1.Position = { x + size, y - size, z - size };
		BottomFace.v2.Position = { x + size, y - size, z + size };
		BottomFace.v3.Position = { x - size, y - size, z + size };

		BottomFace.v0.TC = { 0.0f, 0.0f };
		BottomFace.v1.TC = { 1.0f, 0.0f };
		BottomFace.v2.TC = { 1.0f, 1.0f };
		BottomFace.v3.TC = { 0.0f, 1.0f };

		BottomFace.v0.AtlasData = { 1.0f, 0.0f };
		BottomFace.v1.AtlasData = { 1.0f, 0.0f };
		BottomFace.v2.AtlasData = { 1.0f, 0.0f };
		BottomFace.v3.AtlasData = { 1.0f, 0.0f };

		Data.push_back(BottomFace);


	return Data;
}
