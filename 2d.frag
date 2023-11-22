#version 330 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D imageTexture;
uniform vec4 colorConst;


void main(){
	fragColor = (colorConst.x < .5) ? texture(imageTexture, uv) : colorConst;
}
