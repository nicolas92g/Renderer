//on utilise la version 3.3 core
#version 330 core

//on recupere les vertices du vertex buffer
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUv;

out vec2 uv;
out vec3 normal;
out vec3 pixelPos;

//on recupere nos matrices sous la forme d' uniform buffers
uniform mat4 projection;
uniform mat4 view; 
uniform mat4 model;

//fonction principale
void main(){
	gl_Position = projection * view * model * vec4(pos , 1);		
	
	uv = inUv;
	pixelPos = vec3(model * vec4(pos, 1.0));
	normal = mat3(transpose(inverse(model))) * inNormal;  	
}
