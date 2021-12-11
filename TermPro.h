#define _CRT_SECURE_NO_WARNINGS   
#include <iostream>
#include <gl/glew.h>  
#include <gl/freeglut.h>  
#include <gl/freeglut_ext.h>    
#include <gl/glm/gtc/matrix_transform.hpp>    
#include <gl/glm/glm.hpp>  
#include <gl/glm/ext.hpp>     
#include<string> 
#include<fstream>  
#include<vector>

using namespace std;
using namespace glm;


unsigned int modelLocation;
unsigned int projectionLocation;
unsigned int viewLocation;
unsigned int lightColorLocation;
unsigned int lightPosLocation;
unsigned int ViewPosLocation;
unsigned int objColorLocation;
unsigned int normalLocation;
unsigned int alphaLocation;
GLfloat Angle; //각도  


class objReader {
public:
	int loadObj(const char* filename);
	int loadObj_normalize_center(const char* filename);
	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX, aveY, aveZ;
	float scaleX, scaleY, scaleZ;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
	float scaleAll;

	float sizeX, sizeY, sizeZ;

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector< glm::vec3 > outvertex, outnormal;
	std::vector< glm::vec2 > outuv;
	objReader();
	~objReader();
};

GLuint fragmentShader;
GLuint vertexShader;
GLchar* vertexShaderSource, * fragmentShaderSource;
GLuint shaderID;


char* filetobuf(const string filename)
{
	ifstream inFile(filename, ios::binary);
	vector<char> buf;
	buf.resize(0);
	char temp;
	while (inFile >> noskipws >> temp) {
		buf.push_back(temp);
	}
	buf.push_back(0);
	inFile.close();
	char* resultC = new char[buf.size()];
	for (int i = 0; i < buf.size(); i++)
		resultC[i] = buf[i];
	return resultC;
}
GLvoid make_vertexShaders()
{
	vertexShaderSource = filetobuf("vertexshaderTex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexShaderSource, 0);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "EROOR:vertex shader 컴파일 실패\n" << errorLog << endl;
		exit(-1);
	}
}
GLvoid make_fragmentShaders()
{
	fragmentShaderSource = filetobuf("fragmentshaderTex.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentShaderSource, 0);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR:fragment shader 컴파일 실패\n" << errorLog << endl;
		exit(-1);
	}
}
GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram(); //--- 세이더 프로그램 만들기 
	glAttachShader(ShaderProgramID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(ShaderProgramID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(ShaderProgramID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);

	//Error 잡아내는 코드 
	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		exit(-1);
	}
	glUseProgram(ShaderProgramID);
	return ShaderProgramID;
}



int objReader::loadObj(const char* filename)
{
	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;

			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	std::cout << "minX: " << minX << " minY: " << minY << " minZ: " << minZ << std::endl;
	std::cout << "maxX: " << maxX << " maxY: " << maxY << " maxZ: " << maxZ << std::endl;

	aveX = sumX / vertexIndices.size();
	aveY = sumY / vertexIndices.size();
	aveZ = sumZ / vertexIndices.size();
	scaleX = (1.0 - maxX) * 10 + 1;
	scaleY = (1.0 - maxY) * 10 + 1;
	scaleZ = (1.0 - maxZ) * 10 + 1;

	if (scaleX > scaleY) {
		if (scaleY > scaleZ)
			scaleAll = scaleZ;
		else
			scaleAll = scaleY;
	}
	else if (scaleX < scaleY) {
		if (scaleX < scaleZ)
			scaleAll = scaleX;
		else
			scaleAll = scaleZ;
	}
	std::cout << "aveX: " << aveX << " aveY: " << aveY << " aveZ: " << aveZ << std::endl;

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		outvertex.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 vertex = temp_uvs[uvIndex - 1];
		outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_normals[normalIndex - 1];
		outnormal.push_back(vertex);
	}

	return outvertex.size();
}
int objReader::loadObj_normalize_center(const char* filename)
{
	FILE* objFile;
	fopen_s(&objFile, filename, "rb");
	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;

			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	std::cout << "minX: " << minX << " minY: " << minY << " minZ: " << minZ << std::endl;
	std::cout << "maxX: " << maxX << " maxY: " << maxY << " maxZ: " << maxZ << std::endl;

	aveX = sumX / vertexIndices.size();
	aveY = sumY / vertexIndices.size();
	aveZ = sumZ / vertexIndices.size();
	scaleX = maxX - minX;
	scaleY = maxY - minY;
	scaleZ = maxZ - minZ;

	glm::vec3 temp;

	std::cout << "aveX: " << aveX << " aveY: " << aveY << " aveZ: " << aveZ << std::endl;

	std::cout << "scaleX: " << scaleX << " scaleY: " << scaleY << " scaleZ: " << scaleZ << std::endl;

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		temp = temp_vertices[vertexIndex - 1];
		temp.x = temp.x - minX;
		temp.y = temp.y - minY;
		temp.z = temp.z - minZ;

		temp.x = ((temp.x * 2.0f) / scaleX) - 1.0f;
		temp.y = ((temp.y * 2.0f) / scaleY) - 1.0f;
		temp.z = ((temp.z * 2.0f) / scaleZ) - 1.0f;

		outvertex.push_back(temp);
		//glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		//outvertex.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 vertex = temp_uvs[uvIndex - 1];
		outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_normals[normalIndex - 1];
		outnormal.push_back(vertex);
	}

	return outvertex.size();
}
objReader::objReader() {
	sumX = 0.0;
	sumY = 0.0;
	sumZ = 0.0;
	aveX = 0.0;
	aveY = 0.0;
	aveZ = 0.0;
	scaleX = 0.0;
	scaleY = 0.0;
	scaleZ = 0.0;
	minX = 0.0;
	minY = 0.0;
	minZ = 0.0;
	minY = 0.0;
	minZ = 0.0;
	maxX = 0.0;
	maxY = 0.0;
	maxZ = 0.0;
	scaleAll = 0.0;

	sizeX, sizeY, sizeZ;
}
objReader::~objReader() {

}


//-----------------------------------------------------------------------------
struct loca {
	//큐브가 이동할 위치 
	float x{};
	float y{};
	float z{};

	//큐브가 스케일될 크기   
	float sx{};
	float sy{};
	float sz{};

	//그려졌는지 여부 
	int is{};

	//랜덤하게 올라갈건지 내려갈건지 정하는 변수 
	int rnd{};

	//현재 x,z의 위치 
	float lx{};
	float rx{};
	float dz{};
	float uz{};
};
loca** MDataArr = nullptr;
loca** MDataArr2 = nullptr;
GLuint Stage1[20][20] = { 1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,
						  0,0,0,1,1,1,0,0,0,0,0,1,1,0,1,0,1,0,1,0,
						  0,1,0,0,0,1,1,1,0,1,0,0,0,0,1,0,1,0,1,0,
						  0,1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,0,0,1,0,
						  0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,1,0,
						  1,1,0,1,0,0,0,1,0,1,0,1,1,1,1,0,1,0,1,0,
						  0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,1,0,1,1,
						  1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,0,0,0,
						  0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,
						  0,0,0,1,1,1,1,1,1,1,1,1,0,1,0,0,1,1,1,1,
						  1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,
						  0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,1,1,0,
						  1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,0,0,1,0,
						  0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,
						  1,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,
						  0,1,1,1,0,0,1,0,0,1,0,1,0,1,1,1,0,1,1,0,
						  0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,
						  0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,
						  1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,1,1, };
GLuint Stage2[20][20] = { 0,0,1,0,0,0,0,0,0,1,0,1,0,1,0,1,1,0,1,0,
						  1,0,1,0,1,0,1,0,0,0,0,1,0,1,0,1,0,0,0,0,
						  1,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1,
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						  1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
						  1,1,1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,
						  0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						  1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,
						  0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,
						  0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,
						  0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,
						  0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,0,
						  0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
						  0,0,1,0,0,0,0,0,0,1,1,0,1,1,1,0,1,1,1,1,
						  0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
						  1,1,1,0,1,0,0,1,1,1,1,1,1,1,1,1,1,0,1,0,
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						  1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0 };

//GLuint Stage1[20][20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
//GLuint Stage2[20][20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

//로봇& 몬스터에 필요한 데이터 
glm::mat4 TlLeg = glm::mat4(1.0f); //Robot 왼쪽 다리 변환      
glm::mat4 TrLeg = glm::mat4(1.0f); //Robot 오른쪽 다리 변환    
glm::mat4 TlArm = glm::mat4(1.0f); //Robot 왼쪽 팔 변환      
glm::mat4 TrArm = glm::mat4(1.0f); //Robot 오른쪽 팔 변환     

glm::mat4 TT_Robot = glm::mat4(1.0f); //Robot전체 이동  
glm::mat4 TR_Robot = glm::mat4(1.0f); //Robot전체 회전   
glm::mat4 TJ_Robot = glm::mat4(1.0f); //Robot전체 점프

glm::mat4 TT_Monster[8];
glm::mat4 TR_Monster[8];
glm::mat4 TJ_Monster[8];


glm::mat4 location = glm::mat4(1.0f);
//-------------------------------------------------------------

float moving1, moving2, moving3, moving4;
int leftArmtype, rightArmtype, leftlegtype, rightlegtype;
char current = '0';
//로봇의 위치
float RobotLX = -0.34;
float RobotRX = -0.31;
float RobotUZ = 0.49;
float RobotDZ = 0.46;
float RobotY = 0.52;

//-------------------------------------------------
GLfloat Robot_X = -0.325;
GLfloat Robot_Y = 0.52;
GLfloat Robot_Z = 0.475;
GLuint Floor_state = 2; //로봇의 층 위치임 -> 1층으로 내려가면 이것도 1로 바꿔줘야 함 
//------------------------------------------------
GLfloat tel_x = -0.33f;
GLfloat tel_z = -0.43f;
// 텔레포트박스 좌표값
GLfloat exit_x = 0.0f;
GLfloat exit_z = 0.0f;
// exit박스 좌표값
GLfloat box_x = -0.26f;
GLfloat box_z = 0.43f;
GLfloat R_y = 0.5f;
GLfloat R_x = -0.3;
GLfloat R_z = 0.5;
//GLfloat R_y = 0.1f;

// 로봇 y좌표값및 이동값
bool Bad_ending = false;
bool Happy_ending = false;
bool Game_reset = false;
bool tel_check = false;