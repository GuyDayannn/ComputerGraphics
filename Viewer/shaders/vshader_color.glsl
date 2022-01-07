#version 330 core

#define LIGHTS_MAX 10
#define UV 0
#define PLANE 1
#define CYLINDER 2
#define SPHERE 3


layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 modelColor;
uniform int texType;
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

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;
	mcolor = modelColor;

	for(int i = 0; i < lightCount; i++)
	{
		lightPos[i] = vec3(lightTransfomation[i] * vec4(0.0f, 0.0f, 0.0f, 1.0f));
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

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);
}