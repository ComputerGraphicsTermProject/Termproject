#version 330 core
in vec3 FragPos; 
in vec3 Normal;  
in vec2 TexCoord;

out vec4 FragColor;
uniform vec3 lightPos; //--- 조명의 위치   
uniform vec3 lightColor; //--- 조명의 색   
uniform vec3 viewPos;    //시선 위치   
uniform vec3 objectColor; //obj 색   
uniform vec3 Alpha;//알파값 
uniform sampler2D outTexture; 


void main(){ 
float ambientLight = 0.6;//--- 주변 조명 계수        
vec3 ambient = ambientLight * lightColor;//--- 주변 조명 값 

vec3 norm = normalize(Normal); //--- 노말값을 정규화한다.  
vec3 lightDir = normalize(lightPos-FragPos); //--- 표면과 조명의 위치로 조명의 방향을 결정한다.
float diffuseLight = max(dot(norm, lightDir), 0.0);//--- N과 L의 내적 값으로 강도 조절 (음의 값을 가질 수 없게 한다.)
vec3 diffuse = diffuseLight * lightColor; //--- 산란반사조명값=산란반사값*조명색상값

int shininess = 128;   
vec3 viewDir = normalize(viewPos - FragPos);   
vec3 reflectDir = reflect(-lightDir, norm);  
float specularLight = max(dot(viewDir, reflectDir),0.0); 
specularLight = pow(specularLight, shininess);
vec3 specular = specularLight*lightColor; 
 
vec3 result = (ambient + diffuse + specular) * objectColor; //--- 최종 조명 설정된 픽셀 색상=(주변조명+산란반사조명)*객체 색상
FragColor = vec4(result, Alpha);        
FragColor = texture(outTexture,TexCoord)*FragColor;
}  