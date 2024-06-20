#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

struct Light {
	vec3 position;
	vec3 direction;
	vec3 intensity;
	vec3 ambientIntensity;
};

struct BlinnPhongMaterial {
	vec3 ambient; // ka
	vec3 diffuse; // kd
	vec3 specular; // ks
	float shininess; // p
	vec4 baseColor;
	sampler2D baseColorTexture;
};

uniform vec3 u_ViewPos;

uniform Light u_DirectionalLights[4];
uniform Light u_PointLights[64];
uniform int u_NumPointLights;
uniform int u_NumDirectionalLights;

uniform BlinnPhongMaterial u_Material;

void main()
{
	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
	FragColor = texture(u_Material.baseColorTexture, fs_in.TexCoords);
	return;
	vec3 viewDir = normalize(u_ViewPos - fs_in.FragPos);

	for (int i = 0; i < u_NumPointLights; i++)
	{
		vec3 tmp = u_PointLights[i].position - fs_in.FragPos;
		vec3 lightDir = normalize(tmp);
		float distance = length(tmp);

		vec3 halfwayDir = normalize(lightDir + viewDir);
		
		// (I / r^2)
		vec3 computIntensity = u_PointLights[i].intensity / (distance * distance);

		// Ambient: ka * Ia
		ambient += u_Material.ambient * u_PointLights[i].ambientIntensity;

		// Diffuse: kd * (I / r^2) * (N.L)
		float diff = max(dot(fs_in.Normal, lightDir), 0.0);
		diffuse += u_Material.diffuse * computIntensity * diff;

		// Specular: ks * (I / r^2) * (N.H)^p
		float spec = pow(max(dot(fs_in.Normal, halfwayDir), 0.0), u_Material.shininess);
		specular +=  u_Material.specular * computIntensity * spec;
	}

	vec3 result = (ambient + diffuse + specular) * texture(u_Material.baseColorTexture, fs_in.TexCoords).rgb;
	FragColor = vec4(result, 1.0);
}