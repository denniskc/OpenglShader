// Vertex Shader – file "minimal.vert"

#version 130

in  vec3 in_Position;
in  vec3 in_Color;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float ang;

out vec3 ex_Color;

void main(void)
{
	//float sinbase = sin(ang+in_Position.x*32)*0.1; 
	//float sinbase2 = sin(ang*2+in_Position.x*32)*0.1; 
	//float cosbase2 = sin(ang*2+in_Position.x*32)*0.1; 
	float dx = in_Position.x - 0;
	float dy = in_Position.y - 0; 
	float dist = sqrt(dx*dx+dy*dy);
	float sinbase = sin(ang+dist*28)*0.1;
	float sinbasex = sin(ang+in_Position.x*48)*0.02; 
	
	vec3 pos1 = vec3(in_Position.x,in_Position.y,sinbase+sinbasex);

	float sin = sin(45);
	float cos = cos(45);
	
	ex_Color = vec3(0.2+sinbase*2+sinbasex,0.2+sinbase*+sinbasex,1+sinbase+sinbasex);
	
	//mat4 mvp = projection*view*model*vec4(pos1,1.0);
	
	//vec4 pos2 = vec4(pos1,1.0)*mvp;
	
	vec4 pos2 = projection*view*model*vec4(pos1,1.0);
	
	gl_Position = pos2;
}