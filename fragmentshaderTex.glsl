#version 330 core
in vec3 FragPos; 
in vec3 Normal;  
in vec2 TexCoord;

out vec4 FragColor;
uniform vec3 lightPos; //--- ������ ��ġ   
uniform vec3 lightColor; //--- ������ ��   
uniform vec3 viewPos;    //�ü� ��ġ   
uniform vec3 objectColor; //obj ��   
uniform vec3 Alpha;//���İ� 
uniform sampler2D outTexture; 


void main(){ 
float ambientLight = 0.6;//--- �ֺ� ���� ���        
vec3 ambient = ambientLight * lightColor;//--- �ֺ� ���� �� 

vec3 norm = normalize(Normal); //--- �븻���� ����ȭ�Ѵ�.  
vec3 lightDir = normalize(lightPos-FragPos); //--- ǥ��� ������ ��ġ�� ������ ������ �����Ѵ�.
float diffuseLight = max(dot(norm, lightDir), 0.0);//--- N�� L�� ���� ������ ���� ���� (���� ���� ���� �� ���� �Ѵ�.)
vec3 diffuse = diffuseLight * lightColor; //--- ����ݻ�����=����ݻ簪*�������

int shininess = 128;   
vec3 viewDir = normalize(viewPos - FragPos);   
vec3 reflectDir = reflect(-lightDir, norm);  
float specularLight = max(dot(viewDir, reflectDir),0.0); 
specularLight = pow(specularLight, shininess);
vec3 specular = specularLight*lightColor; 
 
vec3 result = (ambient + diffuse + specular) * objectColor; //--- ���� ���� ������ �ȼ� ����=(�ֺ�����+����ݻ�����)*��ü ����
FragColor = vec4(result, Alpha);        
FragColor = texture(outTexture,TexCoord)*FragColor;
}  