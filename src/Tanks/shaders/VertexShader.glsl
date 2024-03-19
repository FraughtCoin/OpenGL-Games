#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 object_color;
uniform int hp;

out vec3 color;

void main() {

	color = object_color;
	
	int hpInverse = 3 - hp;
	float remainingRed = 1 - object_color.r;

	color.r = color.r + remainingRed / 3 * hpInverse;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}