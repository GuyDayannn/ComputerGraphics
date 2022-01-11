#version 330 core
#define LIGHTS_MAX 10
#define COLOR 0
#define TEXTURE 1
#define REGULAR false
#define NORMAL_MAP true


struct Material
{
	sampler2D textureMap;
	sampler2D normalMap;
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
uniform bool normalType;
uniform LightMaterial lightMatriel[LIGHTS_MAX]; // array of light properties
uniform int lightCount;
uniform int colType;
uniform vec3 camPos;
uniform bool toonShading;
uniform int toonLevels;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
in vec3 mcolor;
in vec3 lightPos[LIGHTS_MAX];
//in mat3 tTBN;
in vec3 TangentLightPos[LIGHTS_MAX];
in vec3 TangentCamPos;
in vec3 TangentFragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
	vec3 texNormal = vec3(texture(material.normalMap, fragTexCoords));
	texNormal = normalize(texNormal * 2.0f - 1.0f);
	float toonLvl = float(toonLevels);

	vec3 mAmbientColor = material.ambientColor;
	vec3 mDiffuseColor = material.diffuseColor;
	vec3 mSpecularColor = material.specularColor;
	if( colType == TEXTURE)
	{
		mAmbientColor = textureColor;
		mDiffuseColor = textureColor;
		mSpecularColor = textureColor;
	}


	vec3 ambientFColor = vec3(0.0f, 0.0f, 0.0f);
	vec3 diffuseFColor = vec3(0.0f, 0.0f, 0.0f);
	vec3 specularFColor = vec3(0.0f, 0.0f, 0.0f);
	vec3 finalColor = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < lightCount; i++)
	{
		vec3 ambientCo, diffuseCo, specularCo;
		vec3 texlightPntDir, texeyeToPoint, texreflection;
		float texdotPro, texdotProSpec;
		//Ambient
		ambientCo = lightMatriel[i].ambientIntensity * lightMatriel[i].ambientColor * mAmbientColor;

		//Diffuse
		vec3 pntNormal = normalize(fragNormal);
		vec3 lightPntDir = normalize(lightPos[i] - fragPos);
		if(normalType == NORMAL_MAP) 
			texlightPntDir = normalize(TangentLightPos[i] - TangentFragPos); // lightPntDir for normal map
		float dotPro = dot(lightPntDir, pntNormal);
		if(toonShading) // for toonShading
			dotPro = floor(dotPro * toonLvl) / toonLvl;
		if(normalType == NORMAL_MAP) 
		{
			texdotPro = dot(texlightPntDir, texNormal); // dotPro for normal map
			if(toonShading) texdotPro = floor(texdotPro * toonLvl) / toonLvl;
		}
		if(normalType == REGULAR)
			diffuseCo = lightMatriel[i].diffuseIntensity * lightMatriel[i].diffuseColor * dotPro * mDiffuseColor;
		else // NORMAL_MAP
			diffuseCo = lightMatriel[i].diffuseIntensity * lightMatriel[i].diffuseColor * texdotPro * mDiffuseColor;

		//Specular
		vec3 eyeToPoint = normalize(camPos - fragPos);
		if(normalType == NORMAL_MAP) 
			texeyeToPoint = normalize(TangentCamPos - TangentFragPos); // eyeToPoint for normal map
		vec3 reflection = normalize(reflect(-lightPntDir, fragNormal));
		if(normalType == NORMAL_MAP)
			texreflection = normalize(reflect(-texlightPntDir, texNormal)); // reflection for normal map
		float dotProSpec = pow(max(dot(reflection, eyeToPoint), 0.0f), material.shininess);
		if(toonShading) // for toonShading
			dotProSpec = floor(dotProSpec * toonLvl) / toonLvl;
		if(normalType == NORMAL_MAP)
		{
			texdotProSpec = pow(max(dot(texreflection, texeyeToPoint), 0.0f), material.shininess); // dotProSpec for normal map
			if (toonShading) texdotProSpec = floor(texdotProSpec * toonLvl) / toonLvl;
		}
		if(normalType == REGULAR)
			specularCo = lightMatriel[i].specularIntensity * lightMatriel[i].specularColor * dotProSpec * mSpecularColor;
		else
			specularCo = lightMatriel[i].specularIntensity * lightMatriel[i].specularColor * texdotProSpec * mSpecularColor;



		vec3 tempColor = ambientCo + diffuseCo + specularCo;
		finalColor += tempColor;
	}

	frag_color = vec4(finalColor,1);
}
