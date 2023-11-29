#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 pixelPos;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform vec3 diffuseConst;

uniform float ambientStrength = .2;
uniform vec3 lightDir = vec3(.4, .6, -.3);

void main(){
	vec4 diffuseColor;
	if (diffuseConst.x == -1)
		diffuseColor = texture(diffuseTexture, uv);
	else
		diffuseColor = vec4(diffuseConst, 1);

	vec3 norm = normalize(normal);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1);

	fragColor = vec4(diffuseColor.rgb * ( ambientStrength + diffuse), diffuseColor.a);
	//fragColor = vec4(diffuse, 1);
}
