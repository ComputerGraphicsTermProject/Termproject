#version 330 core
layout (location = 0) in vec3 vPos;  
layout (location = 1) in vec3 vNormal;    
layout (location = 2) in vec2 vTexCoord;     

uniform mat4 modelTransform;
uniform mat4 projectionTransform; 
uniform mat4 viewTransform; 

out vec3 FragPos; 
out vec3 Normal; 
out vec3 passColor; 
out vec2 TexCoord;  

void main(){
 gl_Position = projectionTransform*viewTransform*modelTransform*vec4(vPos,1.0); 

 FragPos = vec3(modelTransform * vec4(vPos, 1.0)); 
 Normal = vec3(modelTransform*vec4(vNormal,1.0));   
 TexCoord = vTexCoord;  
}  
