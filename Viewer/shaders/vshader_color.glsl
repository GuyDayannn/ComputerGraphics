#version 330 core

#define LIGHTS_MAX 10
#define UV 0
#define PLANE 1
#define CYLINDER 2
#define SPHERE 3
#define REGULAR false
#define NORMAL_MAP true


layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 tangent;
//layout(location = 4) in vec3 bitangent;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 modelColor;
uniform int texType;
uniform bool normalType;
uniform int colType;
uniform vec3 camPos;
uniform int lightCount;
uniform mat4 lightTransfomation[LIGHTS_MAX];

// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;
out vec3 mcolor;
out vec3 lightPos[LIGHTS_MAX];
//out mat3 tTBN;
out vec3 TangentLightPos[LIGHTS_MAX];
out vec3 TangentCamPos;
out vec3 TangentFragPos;

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;
	mcolor = modelColor;

	mat3 tTBN;
	if(normalType == NORMAL_MAP)
	{
		vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
		//vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
		vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
		T = normalize(T - dot(T, N) * N);
		vec3 B = cross(N, T);
		mat3 TBN = mat3(T, B, N);
		tTBN = transpose(TBN);

		TangentCamPos = tTBN * camPos;
		TangentFragPos = tTBN * fragPos;
	
	}


	for(int i = 0; i < lightCount; i++)
	{
		lightPos[i] = vec3(lightTransfomation[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f));
		if(normalType == NORMAL_MAP) 
			TangentLightPos[i] = tTBN * lightPos[i];
	}


	// Pass the vertex texture coordinates property as it is. Its interpolated value
	// will be avilable for us in the fragment shader
	if (texType == UV)
		fragTexCoords = texCoords;
	else if(texType == PLANE)
		fragTexCoords = vec2(pos.x, pos.z);
	else if(texType == CYLINDER)
	{
		float theta = atan(pos.x, pos.z) + 3.14159265359f;
		fragTexCoords = vec2(theta, pos.y);
	}
	else	//SPHERE
	{
		float phi = atan(pos.y, pos.z);
		float theta = atan(pos.z, pos.x);
		fragTexCoords = vec2(theta, phi);
	}

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);
}