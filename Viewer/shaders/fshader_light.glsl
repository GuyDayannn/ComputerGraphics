#version 330 core
#define LIGHTS_MAX 10

struct Material
{
	sampler2D textureMap;
	float shininess;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

struct LightMaterial
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
};



// We set this field's properties from the C++ code
uniform Material material; //model matriel
uniform vec3 camPos;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
in vec3 mcolor;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	frag_color = vec4(vec3(1,1,1),1);
}
