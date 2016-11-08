// Vertex Shader – file "minimal.vert"

#version 130

in  vec3 in_Position;
in  vec3 in_Color;
uniform float ang;

out vec3 ex_Color;

void main(void)
{
	float sin = sin(ang);
	float cos = cos(ang);
	ex_Color = in_Color;
	
	vec3 pos2 = vec3(in_Position.x,in_Position.y*cos+in_Position.z*sin,in_Position.z*cos+in_Position.y*sin);
	
	gl_Position = vec4(pos2, 1.0);
}