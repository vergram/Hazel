#type vertex
#version 330 core	

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_TransformMatrix;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjectionMatrix * u_TransformMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
	
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	//color = vec4(v_TexCoord, 0.0, 1.0);
	color = texture(u_Texture, v_TexCoord);
}
