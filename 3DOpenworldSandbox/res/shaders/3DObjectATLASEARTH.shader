#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texcoord;
layout(location = 3) in vec3 a_Normal;

layout(location = 4) in vec2 a_texPos;


out vec4 v_Color;
out vec2 v_Texcoord;

out vec2 v_texPos;


//uniform mat4 u_MVP;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
        
void main()
{
    gl_Position = Projection * View * Model * vec4(a_Position,1.0);

    v_Color = a_Color;
    v_Texcoord = a_Texcoord;

    v_texPos = a_texPos;


};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;
    
in vec4 v_Color;
in vec2 v_Texcoord;

in vec2 v_texPos;

uniform sampler2D u_Texture;

void main()
{


    vec2 texSize = vec2(16.0,16.0);
    vec2 atlasSize = vec2(256.0,256.0);
    vec2 SafetyBorderSize = vec2(0.0,0.0);

    vec2 NormalizedTileSize = vec2((texSize.x - SafetyBorderSize.x)/atlasSize.x,(texSize.y - SafetyBorderSize.y)/atlasSize.y);

    vec2 tileOffset = vec2((texSize.x * v_texPos.x)/atlasSize.x,(texSize.y * v_texPos.y)/atlasSize.y);
    //vec2 tileOffset = vec2((16.0 * v_texPos.x)/atlasSize.x,(16.0 * v_texPos.y)/atlasSize.y);

    vec4 texColor = texture(u_Texture,v_Texcoord * NormalizedTileSize + tileOffset);
    color = texColor;
};