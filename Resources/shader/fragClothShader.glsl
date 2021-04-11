#version 330 core
out vec4 FragColor;


struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	// Direction has to be in world coordinates.
	vec3 position;
	vec3 color;

};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;
uniform bool lightEnable;

in vec3 FragPos;
in vec3 wNormal;
in vec4 Color;
in vec2 TexCoord;

uniform sampler2D texture2D;




void main()
{


    vec3 ambient = light.color * material.ambient;

    vec3 norm = normalize(wNormal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color  * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color  * (spec * material.specular);  

    vec3 result = ambient + diffuse + specular;
    vec4 r2 = vec4(result, 1.0);
	if(lightEnable){
			FragColor =  texture(texture2D, TexCoord) * Color * r2;
	}else{
			FragColor =  texture(texture2D, TexCoord) * Color ;
	}



}