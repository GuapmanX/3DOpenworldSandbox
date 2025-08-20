#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec2 a_AtlasCoord;

out vec2 v_Texcoord;
out vec2 v_Atlascoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	v_Texcoord = a_TexCoord;
	v_Atlascoord = a_AtlasCoord;
	gl_Position = Projection * View * Model * vec4(a_Position,1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_Texcoord;
in vec2 v_Atlascoord;

uniform sampler2D u_Texture;

void main()
{

	vec2 texSize = vec2(225.0,225.0);
    vec2 atlasSize = vec2(900.0,675.0);
	vec2 SafetyBorderSize = vec2(1.0,1.0);

    vec2 NormalizedTileSize = vec2((texSize.x - SafetyBorderSize.x)/atlasSize.x,(texSize.y - SafetyBorderSize.y)/atlasSize.y);

    vec2 tileOffset = vec2((texSize.x * v_Atlascoord.x)/atlasSize.x,(texSize.y * v_Atlascoord.y)/atlasSize.y);

	vec2 SafetyBorderOffset = vec2(((SafetyBorderSize.x/2.0)/atlasSize.x), ((SafetyBorderSize.y/2.0)/atlasSize.y));

	vec4 tex_color = texture(u_Texture, v_Texcoord * NormalizedTileSize + tileOffset + SafetyBorderOffset);

	color = tex_color;
}