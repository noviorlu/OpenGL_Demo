#version 330 core

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform vec3 u_ViewPos;

out vec4 o_Color;

struct Light {
	vec3 position;
	vec3 direction;
	vec3 lightColor;
	float intensity;
};

struct BlinnPhongMaterial {
	vec3 ambient; // ka
	vec3 diffuse; // kd
	vec3 specular; // ks
	float shininess; // p
	vec4 baseColor;
	sampler2D baseColorTexture;
	bool useBaseColorTexture;
};

// light properties
uniform Light u_DirectionalLights[4];
uniform Light u_PointLights[64];
uniform int u_PointLightCount;
uniform int u_DirectionalLightCount;

// Ambient light properties
uniform vec3 u_AmbientLight; // Ambient light color
uniform float u_AmbientIntensity; // Ambient light intensity


uniform BlinnPhongMaterial u_Material;

void main()
{
	// base color
	vec3 baseColor = vec3(1.0);
	if(u_Material.useBaseColorTexture)
		baseColor = texture(u_Material.baseColorTexture, fs_in.TexCoords).rgb;
	else
		baseColor = u_Material.baseColor.rgb;

	// Initialize the result with the ambient component
	vec3 ambient = u_Material.ambient * u_AmbientLight * u_AmbientIntensity;
	vec3 resultColor = ambient;

	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(u_ViewPos - fs_in.FragPos);

	for (int i = 0; i < u_PointLightCount; i++)
	{
		vec3 lightPos = u_PointLights[i].position;
		vec3 lightColor = u_PointLights[i].lightColor;
		float intensity = u_PointLights[i].intensity;

		// Light direction and Distance
		vec3 lightDir = normalize(lightPos - fs_in.FragPos);
		float distance = length(lightPos - fs_in.FragPos);
		//float attenuation = intensity / (1.0 + 0.09 * distance + 0.032 * distance * distance);
		float attenuation = intensity / (distance * distance);

		// Diffuse: kd * (I / r^2) * (N.L)
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = u_Material.diffuse * (lightColor * attenuation) * diff;

		// Specular: ks * (I / r^2) * (N.H)^p
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(norm, halfwayDir), 0.0), u_Material.shininess);
		vec3 specular =  u_Material.specular * (lightColor * attenuation) * spec;

		resultColor += (diffuse + specular);
	}

	o_Color = vec4(resultColor * baseColor, 1.0);
}