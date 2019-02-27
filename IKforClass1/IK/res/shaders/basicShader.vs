#version 120

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 color;
attribute vec3 weight;



varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;
varying vec3 weight0;
varying vec3 position0;

uniform mat4 MVP;
uniform mat4 Normal;
uniform mat4[20] T;
uniform int index;

/***********linear deformation*************/

mat4 calcDeformation(){

	if(index == 0){
		return (weight.y* T[index] + weight.z * T[index+1]);
	}
	else if( index == 4){
			return (weight.x* T[index-1] + weight.y * T[index]);
	}
	else{
		return (weight.x* T[index-1] + weight.y * T[index] + weight.z * T[index+1]);
	}
}

void main()
{
	//gl_Position = MVP * vec4(position, 1.0);
	if(index<5){
		gl_Position = calcDeformation() * vec4(position, 1.0);
		}
	else
		gl_Position = MVP * vec4(position, 1.0);
	texCoord0 = texCoord;
	color0 = color;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
	weight0 = weight;
	position0 = position;
}
