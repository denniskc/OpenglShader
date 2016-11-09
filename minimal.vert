// Vertex Shader – file "minimal.vert"

#version 130

in  vec3 in_Position;
in  vec3 in_Color;
uniform float ang;

out vec3 ex_Color;

void main(void)
{
	float sinbase = sin(ang+in_Position.x*32)*0.1; 
	vec3 pos1 = vec3(in_Position.x,in_Position.y,sinbase);

	float sin = sin(45);
	float cos = cos(45);
	
	ex_Color = vec3(0.4+sinbase,0.4+sinbase,1+sinbase);
	
	vec3 pos2 = vec3(pos1.x,pos1.y*cos+pos1.z*sin,pos1.z*cos+pos1.y*sin);
	
	gl_Position = vec4(pos2, 1.0);
}