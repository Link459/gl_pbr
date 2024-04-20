#version 410 core
out vec4 FragColor;

in vec3 color;
in vec3 normal;
in vec3 frag_pos;

uniform vec3 view_pos;

struct Material {
  	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    vec3 ambient = light.ambient * material.ambient;

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(norm,light_dir),0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

  	vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result =  (ambient + diffuse + specular) * color;
	FragColor = vec4(result, 1.0);
}
