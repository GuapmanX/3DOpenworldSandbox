#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texcoord;
layout(location = 3) in vec3 a_Normal;

layout(location = 4) in vec2 a_atlSize;
layout(location = 5) in vec2 a_texSize;
layout(location = 6) in vec2 a_texPos;

out vec4 v_Color;
out vec2 v_Texcoord;

out vec2 v_atlSize;
out vec2 v_texSize;
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

    v_atlSize = a_atlSize;
    v_texSize = a_texSize;
    v_texPos = a_texPos;


};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;
    
in vec4 v_Color;
in vec2 v_Texcoord;

in vec2 v_atlSize;
in vec2 v_texSize;
in vec2 v_texPos;

uniform sampler2D u_Texture;

void main()
{

    //vec2 tileSize = vec2(16.0/256.0,16.0/256.0);
    //vec2 tileOffset = vec2((16.0 * 0.0)/256.0,(16.0 * 15.0)/256.0);

    vec2 tileSize = vec2(v_texSize.x/v_atlSize.x,v_texSize.y/v_atlSize.y);
    vec2 tileOffset = vec2((v_texSize.x * v_texPos.x)/v_atlSize.x,(v_texSize.y * v_texPos.y)/v_atlSize.y);

    vec4 texColor = texture(u_Texture,v_Texcoord * tileSize + tileOffset);
    color = texColor;
};