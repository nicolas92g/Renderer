#version 330 core


layout(location = 0) in vec2 pos;

out vec2 uv;

uniform mat4 projection;
uniform mat4 model;

void main(){
	gl_Position = projection * model * vec4(pos, 0, 1);	
	
	uv = (pos + .5) * vec2(1., -1.);
}