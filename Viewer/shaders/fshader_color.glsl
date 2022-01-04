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
uniform LightMaterial lightMatriel[LIGHTS_MAX]; // array of light properties
uniform int lightCount;
uniform vec3 camPos;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
in vec3 mcolor;
in vec3 lightPos[LIGHTS_MAX];
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	vec3 ambientFColor = vec3(0.0f, 0.0f, 0.0f);
	vec3 diffuseFColor = vec3(0.0f, 0.0f, 0.0f);
	vec3 specularFColor = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < lightCount; i++)
	{
		//Ambient
		vec3 ambientCo = lightMatriel[i].ambientIntensity * lightMatriel[i].ambientColor * material.ambientColor;
		//Diffuse
		vec3 pntNormal = normalize(fragNormal);
		vec3 lightPntDir = normalize(lightPos[i] - fragPos);
		float dotPro = dot(lightPntDir, pntNormal);
		vec3 diffuseCo = lightMatriel[i].diffuseIntensity * lightMatriel[i].diffuseColor * dotPro * material.diffuseColor;
		//Specular
		vec3 eyeToPoint = normalize(camPos - fragPos);
		vec3 reflection = normalize(reflect(-lightPntDir, fragNormal));
		float dotProSpec = pow(max(dot(reflection, eyeToPoint), 0.0f), material.shininess);
		vec3 specularCo = lightMatriel[i].specularIntensity * lightMatriel[i].specularColor * dotProSpec * material.specularColor;

		ambientFColor += ambientCo;
		diffuseFColor += diffuseCo;
		specularFColor += specularCo;
	}

	vec3 finalColor = ambientFColor + diffuseFColor + specularFColor;


	frag_color = vec4(finalColor,1);
}
