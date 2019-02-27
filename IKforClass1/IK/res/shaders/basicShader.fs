#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;
varying vec3 weight0;
varying vec3 position0;

uniform sampler2D sampler;
uniform vec3 [2]lightDirection;
uniform vec3 [2]lightColor;
uniform vec4 ambient;
uniform int index;
uniform vec3 eye;


vec3 CalcDiffuse(vec3 L){

	vec3 kd = vec3(0.8f,0.4f,0.5f);
	vec3 diffuse = kd*max(dot(normal0,normalize(-L)),0);
	return diffuse;
}

vec3 calcSpecular(vec3 L){

	vec3 Ks = vec3(0.7,0.7,0.7);
	vec3 u = normalize(L);
	vec3 R = normalize(u - 2*normal0*dot(u,normal0));
	vec3 V = normalize(eye-position0);
	vec3 specular = Ks*max(pow(dot(V, R),50), 0);
	specular = clamp(specular, vec3(0.0,0.0,0.0),vec3(1.0,1.0,1.0));

	return specular;


}

void main()
{
	vec3 L = vec3(0.0f,0.5f,-1.0f);
	
    vec3 tmp = dot(-lightDirection[0], normal0) * color0 ;
	if(index >4)
	{
		gl_FragColor = clamp((texture2D(sampler, texCoord0) *clamp(vec4(tmp,1.0), 0.0, 1.0) +  vec4(CalcDiffuse(L),1.0) + vec4(calcSpecular(L),1.0)+ ambient),0.0,0.85);
	//gl_FragColor=texture2D(sampler,texCoord0);
	}
	else
		gl_FragColor = clamp((texture2D(sampler, texCoord0) *clamp(vec4(tmp,1.0), 0.0, 1.0) ),0.0,0.85);

	//gl_FragColor = clamp(vec4(color0,1),0.0,1.0);
	//gl_FragColor = vec4(normal0,1.0);

}








