// Fragment Shader – file "minimal.frag"

#version 130

precision highp float; // needed only for version 1.30

in  vec3 ex_Color;
out vec4 out_Color;

void main(void)
{
	vec3 color = vec3(0,0,0);

	color.x = pow(ex_Color.x,1)*1.8;
	color.y = pow(ex_Color.y,1)*0.5;
	color.z = pow(ex_Color.z,1)*0.8;
	
	//float media =(ex_Color.x+ex_Color.y+ex_Color.z)/3;
	
	//color.x = pow(media,4);
	//color.y = pow(media,4);
	//color.z = pow(media,4);

	out_Color = vec4(color,1.0);
	
	// Try replacing the above with the following:
	//vec3 tmp_Color;
	//tmp_Color = ex_Color.rrr;	
	//out_Color = vec4(tmp_Color,1.0);
}
