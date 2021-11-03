#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h>   
#include <gl/glm/gtc/matrix_transform.hpp> 
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp> 
#include <math.h> 

using namespace std;
//vertex shader "code ����"
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 vPos;\n"
"layout (location = 1) in vec3 vColor;\n"
"uniform mat4 modelTransform;\n"
"uniform mat4 projectionTransform;\n"
"uniform mat4 viewTransform;\n"

"out vec3 passColor;\n"
"void main()\n"
"{\n"
"gl_Position = projectionTransform*viewTransform*modelTransform*vec4(vPos,1.0);\n"
"passColor = vColor;\n"
"}\0";


//fragment shader "code ����"
const GLchar* fragmentShaderSource = "#version 330 core\n"
"in vec3 passColor;\n"
"out vec4 fragmentColor;\n"
"void main() \n"
"{\n"
"fragmentColor = vec4(passColor,1.0); \n"
"} \0";

//fragment shader "code ����"
const GLchar* fragmentShaderSource2 = "#version 330 core\n"
"in vec3 passColor;\n"
"out vec4 fragmentColor;\n"
"void main() \n"
"{\n"
"fragmentColor = vec4(0,0.2,1,1); \n" 
"} \0";

struct state {
    char mode = '0';
    char current = '0';
    int isb = 0;
    int isB = 0;
    int ism = 0;
    int isM = 0;
    int ist = 0;
    int isr = 0; 
    int isR = 0; 
    int isy = 0; 
    int isa = 0;
    int isA = 0;
};
state s;

//ũ���� ��ġ�� �迭�� ���� 
GLfloat Crain[36][6] = {
    {-0.1,0.7,0.1,1,1,0},
    {-0.1,0.6,0.1,1,1,0},
    {0.1,0.6,0.1,1,1,0},

    {-0.1,0.6,0.1,1,1,0},
    {0.1,0.7,0.1,1,1,0},
    {0.1,0.6,0.1,1,1,0}, //�ո�

    {-0.1,0.7,-0.1,1,1,0},
    {-0.1,0.6,-0.1,1,1,0},
    {0.1,0.7,-0.1,1,1,0},

    {0.1,0.7,-0.1,1,1,0},
    {-0.1,0.6,-0.1,1,1,0},
    {0.1,0.6,-0.1,1,1,0},//�޸� 

   
    {-0.1,0.7,0.1,1,1,0},
    {-0.1,0.6,0.1,1,1,0},
    {-0.1,0.7,-0.1,1,1,0},

    {-0.1,0.7,-0.1,1,1,0},
    {-0.1,0.6,0.1,1,1,0},
    {-0.1,0.6,-0.1,1,1,0}, //���� ����

    {0.1,0.7,0.1,1,1,0},
    {0.1,0.6,0.1,1,1,0},
    {0.1,0.6,-0.1,1,1,0},

    {0.1,0.7,0.1,1,1,0},
    {0.1,0.6,-0.1,1,1,0},
    {0.1,0.7,-0.1,1,1,0}, //������ ����

    {-0.1,0.7,-0.1,1,1,0},
    {-0.1,0.7,0.1,1,1,0},
    {0.1,0.7,0.1,1,1,0},

    {-0.1,0.7,-0.1,1,1,0},
    {0.1,0.7,0.1,1,1,0},
    {0.1,0.7,-0.1,1,1,0}, //����

    {-0.1,0.6,-0.1,1,1,0},
    {-0.1,0.6,0.1,1,1,0},
    {0.1,0.6,-0.1,1,1,0},

    {0.1,0.6,-0.1,1,1,0},
    {-0.1,0.6,0.1,1,1,0},
    {0.1,0.6,0.1,1,1,0}, //�Ʒ���  
};     
GLfloat CrainBotom[36][6] = {   
    {-0.15,0.6,0.15,1,0,0},
    {-0.15,0.5,0.15,1,0,0},
    {0.15,0.5,0.15,1,0,0},

    {-0.15,0.5,0.15,1,0,0},
    {0.15,0.6,0.15,1,0,0},
    {0.15,0.5,0.15,1,0,0}, //�ո�

    {-0.15,0.6,-0.15,1,0,0},
    {-0.15,0.5,-0.15,1,0,0},
    {0.15,0.6,-0.15,1,0,0},

    {0.15,0.6,-0.15,1,0,0},
    {-0.15,0.5,-0.15,1,0,0},
    {0.15,0.5,-0.15,1,0,0},//�޸� 


    {-0.15,0.6,0.15,1,0,0},
    {-0.15,0.5,0.15,1,0,0},
    {-0.15,0.6,-0.15,1,0,0},

    {-0.15,0.6,-0.15,1,0,0},
    {-0.15,0.5,0.15,1,0,0},
    {-0.15,0.5,-0.15,1,0,0}, //���� ����

    {0.15,0.6,0.15,1,0,0},
    {0.15,0.5,0.15,1,0,0},
    {0.15,0.5,-0.15,1,0,0},

    {0.15,0.6,0.15,1,0,0},
    {0.15,0.5,-0.15,1,0,0},
    {0.15,0.6,-0.15,1,0,0}, //������ ����

    {-0.15,0.6,-0.15,1,0,0},
    {-0.15,0.6,0.15,1,0,0},
    {0.15,0.6,0.15,1,0,0},

    {-0.15,0.6,-0.15,1,0,0},
    {0.15,0.6,0.15,1,0,0},
    {0.15,0.6,-0.15,1,0,0}, //����

    {-0.15,0.5,-0.15,1,0,0},
    {-0.15,0.5,0.15,1,0,0},
    {0.15,0.5,-0.15,1,0,0},

    {0.15,0.5,-0.15,1,0,0},
    {-0.15,0.5,0.15,1,0,0}, 
    {0.15,0.5,0.15,1,0,0}, //�Ʒ���  
}; 
GLfloat Arm1[36][6] = { 
    {-0.05,0.8,0.03,0,0,1},
    {-0.05,0.7,0.03,0,0,1},
    {-0.03,0.7,0.03,0,0,1},

    {-0.05,0.7,0.03,0,0,1},
    {-0.03,0.8,0.03,0,0,1},
    {-0.03,0.7,0.03,0,0,1}, //�ո�

    {-0.05,0.8,0.05,0,0,1},
    {-0.05,0.7,0.05,0,0,1},
    {-0.03,0.8,0.05,0,0,1},

    {-0.03,0.8,0.05,0,0,1},
    {-0.05,0.7,0.05,0,0,1},
    {-0.03,0.7,0.05,0,0,1},//�޸� 


    {-0.05,0.8,0.03,0,0,1},
    {-0.05,0.7,0.03,0,0,1},
    {-0.05,0.8,0.05,0,0,1},

    {-0.05,0.8,0.05,0,0,1},
    {-0.05,0.7,0.03,0,0,1},
    {-0.05,0.7,0.05,0,0,1}, //���� ����

    {-0.03,0.8,0.03,0,0,1},
    {-0.03,0.7,0.03,0,0,1},
    {-0.03,0.7,0.05,0,0,1},

    {-0.03,0.8,0.03,0,0,1},
    {-0.03,0.7,0.05,0,0,1},
    {-0.03,0.8,0.05,0,0,1}, //������ ����

    {-0.05,0.8,0.05,0,0,1},
    {-0.05,0.8,0.03,0,0,1},
    {-0.03,0.8,0.03,0,0,1},

    {-0.05,0.8,0.05,0,0,1},
    {-0.03,0.8,0.03,0,0,1},
    {-0.03,0.8,0.05,0,0,1}, //����

    {-0.05,0.7,0.05,0,0,1},
    {-0.05,0.7,0.03,0,0,1},
    {-0.03,0.7,0.05,0,0,1},

    {-0.03,0.7,0.05,0,0,1},
    {-0.05,0.7,0.03,0,0,1},
    {-0.03,0.7,0.03,0,0,1}, //�Ʒ���  
};
GLfloat Arm2[36][6] = {
    {0.03,0.8,0.05,0,0,1},
    {0.03,0.7,0.05,0,0,1},
    {0.05,0.7,0.05,0,0,1},

    {0.03,0.7,0.05,0,0,1},
    {0.05,0.8,0.05,0,0,1},
    {0.05,0.7,0.05,0,0,1}, //�ո�

    {0.03,0.8,0.03,0,0,1},
    {0.03,0.7,0.03,0,0,1},
    {0.05,0.8,0.03,0,0,1},

    {0.05,0.8,0.03,0,0,1},
    {0.03,0.7,0.03,0,0,1},
    {0.05,0.7,0.03,0,0,1},//�޸� 


    {0.03,0.8,0.05,0,0,1},
    {0.03,0.7,0.05,0,0,1},
    {0.03,0.8,0.03,0,0,1},

    {0.03,0.8,0.03,0,0,1},
    {0.03,0.7,0.05,0,0,1},
    {0.03,0.7,0.03,0,0,1}, //���� ����

    {0.05,0.8,0.05,0,0,1},
    {0.05,0.7,0.05,0,0,1},
    {0.05,0.7,0.03,0,0,1},

    {0.05,0.8,0.05,0,0,1},
    {0.05,0.7,0.03,0,0,1},
    {0.05,0.8,0.03,0,0,1}, //������ ����

    {0.03,0.8,0.03,0,0,1},
    {0.03,0.8,0.05,0,0,1},
    {0.05,0.8,0.05,0,0,1},

    {0.03,0.8,0.03,0,0,1},
    {0.05,0.8,0.05,0,0,1},
    {0.05,0.8,0.03,0,0,1}, //����

    {0.03,0.7,0.03,0,0,1},
    {0.03,0.7,0.05,0,0,1},
    {0.05,0.7,0.03,0,0,1},

    {0.05,0.7,0.03,0,0,1},
    {0.03,0.7,0.05,0,0,1},
    {0.05,0.7,0.05,0,0,1}, //�Ʒ���  
};

//Floor�� ��ġ�� �迭�� ����  
GLfloat Data[36][6] = {
    {-0.5,0.5,0.5,0.6,0.6,0.6},
    {-0.5,0.3,0.5,0.6,0.6,0.6},
    {0.5,0.3,0.5,0.6,0.6,0.6},

    {-0.5,0.5,0.5,0.6,0.6,0.6},
    {0.5,0.3,0.5,0.6,0.6,0.6},
    {0.5,0.5,0.5,0.6,0.6,0.6}, //�ո� 

    {-0.5,0.5,-0.5,0.6,0.6,0.6},
    {-0.5,0.3,-0.5,0.6,0.6,0.6},
    {0.5,0.3,-0.5,0.6,0.6,0.6},

    {-0.5,0.5,-0.5,0.6,0.6,0.6},
    {0.5,0.3,-0.5,0.6,0.6,0.6},
    {0.5,0.5,-0.5,0.6,0.6,0.6}, //�޸� 

    {-0.5,0.5,-0.5,0.6,0.6,0.6},
    {-0.5,0.3,-0.5,0.6,0.6,0.6},
    {-0.5,0.5,0.5,0.6,0.6,0.6},

    {-0.5,0.5,0.5,0.6,0.6,0.6},
    {-0.5,0.3,-0.5,0.6,0.6,0.6},
    {-0.5,0.3,0.5,0.6,0.6,0.6}, //���� ����

    {0.5,0.5,0.5,0.6,0.6,0.6},
    {0.5,0.3,0.5,0.6,0.6,0.6},
    {0.5,0.3,-0.5,0.6,0.6,0.6},

    {0.5,0.5,0.5,0.6,0.6,0.6},
    {0.5,0.3,-0.5,0.6,0.6,0.6},
    {0.5,0.5,-0.5,0.6,0.6,0.6},  //������ ����

    {-0.5,0.5,-0.5,0.6,0.6,0.6},
    {-0.5,0.5,0.5,0.6,0.6,0.6},
    {0.5,0.5,0.5,0.6,0.6,0.6},

    {-0.5,0.5,-0.5,0.6,0.6,0.6},
    {0.5,0.5,0.5,0.6,0.6,0.6},
    {0.5,0.5,-0.5,0.6,0.6,0.6}, //����

    {-0.5,0.3,0.5,0.6,0.6,0.6},
    {-0.5,0.3,-0.5,0.6,0.6,0.6},
    {0.5,0.3,0.5,0.6,0.6,0.6},

    {0.5,0.3,0.5,0.6,0.6,0.6},
    {-0.5,0.3,-0.5,0.6,0.6,0.6},
    {0.5,0.3,-0.5,0.6,0.6,0.6} //�ظ�  
};

GLuint fragmentShader;
GLuint vertexShader;
GLuint shaderID;  

GLuint VAO[5], VBO[5];  
GLvoid InitBuffer() {
    //Floor 
    glGenVertexArrays(5, &VAO[0]);
    glBindVertexArray(VAO[0]);
    glGenBuffers(5, &VBO[0]);  
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);  
    //--- vertex positions ������ �Է�. 
    glBufferData(GL_ARRAY_BUFFER, (36 * 6) * sizeof(GLfloat), Data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //--- vertex color ������ �Է�.    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //ũ���� ���� 
    glGenVertexArrays(5, &VAO[1]);
    glBindVertexArray(VAO[1]);
    glGenBuffers(5, &VBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    //--- vertex positions ������ �Է�. 
    glBufferData(GL_ARRAY_BUFFER, (36 * 6) * sizeof(GLfloat), Crain, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //--- vertex color ������ �Է�.    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //ũ���� �����ϴܺ� 
    glGenVertexArrays(5, &VAO[2]);
    glBindVertexArray(VAO[2]);
    glGenBuffers(5, &VBO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    //--- vertex positions ������ �Է�. 
    glBufferData(GL_ARRAY_BUFFER, (36 * 6) * sizeof(GLfloat), CrainBotom, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //--- vertex color ������ �Է�.    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //ũ���� �� 1
    glGenVertexArrays(5, &VAO[3]);
    glBindVertexArray(VAO[3]);
    glGenBuffers(5, &VBO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    //--- vertex positions ������ �Է�. 
    glBufferData(GL_ARRAY_BUFFER, (36 * 6) * sizeof(GLfloat), Arm1, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //--- vertex color ������ �Է�.    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //ũ���� �� 2
    glGenVertexArrays(5, &VAO[4]);
    glBindVertexArray(VAO[4]);
    glGenBuffers(5, &VBO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    //--- vertex positions ������ �Է�. 
    glBufferData(GL_ARRAY_BUFFER, (36 * 6) * sizeof(GLfloat), Arm2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //--- vertex color ������ �Է�.    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
} 
//vertex shader��ü ���� 
GLvoid make_vertexShaders()
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        cerr << "EROOR:vertex shader ������ ����\n" << errorLog << endl;
        exit(-1);
    }
}
//fragment shader ��ü ����
GLvoid make_fragmentShaders()
{
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        cerr << "ERROR:fragment shader ������ ����\n" << errorLog << endl;
        exit(-1);
    }

}
//���̴� ���α׷� ����  
GLuint make_shaderProgram()
{
    GLuint ShaderProgramID;
    ShaderProgramID = glCreateProgram(); //--- ���̴� ���α׷� ����� 
    glAttachShader(ShaderProgramID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
    glAttachShader(ShaderProgramID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
    glLinkProgram(ShaderProgramID); //--- ���̴� ���α׷� ��ũ�ϱ�
    glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
    glDeleteShader(fragmentShader);

    //Error ��Ƴ��� �ڵ� 
    GLint result;
    GLchar errorLog[512];

    glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
        cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
        exit(-1);
    }
    glUseProgram(ShaderProgramID);
    return ShaderProgramID;
}


glm::mat4 FloorModel = glm::mat4(1.0f);
glm::mat4 CrainModel = glm::mat4(1.0f);  
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f); 
glm::mat4 model = glm::mat4(1.0f); 
glm::mat4 t = glm::mat4(1.0f); //�̵� ���  
glm::mat4 r = glm::mat4(1.0f); //ȸ�� ���   
glm::mat4 ar1 = glm::mat4(1.0f); //�� ȸ�� ���    
glm::mat4 ar2 = glm::mat4(1.0f); //�� ȸ�� ��� 
glm::mat4 camera = glm::mat4(1.0f);  //ī�޶� ȸ�� ���          
glm::mat4 rAll = glm::mat4(1.0f); //��ü �� ȸ�� ���         

GLfloat cx = 0.0f;
GLfloat cy = 1.0f;
GLfloat cz = 0.5f;

GLfloat c2x = 0.0f; 
GLfloat c2y = 0.3f; 
GLfloat c2z = 0.0f;
 
glm::vec3 cameraPos = glm::vec3(cx,cy,cz);          
glm::vec3 cameraDirection = glm::vec3(c2x, c2y, c2z);             
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       

unsigned int modelLocation;
unsigned int projectionLocation;
unsigned int viewLocation;
GLfloat angle; 

GLvoid a() {
    glEnable(GL_CULL_FACE); 
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);  
    glEnable(GL_DEPTH_TEST); 
}
GLvoid Pro() {
    glUseProgram(shaderID);
    projection = glm::mat4(1.0f);
    angle = glm::radians(30.0f); 
    projection = glm::perspective(angle, (float)800 / (float)600, 1.0f, 50.0f);
    projection = glm::translate(projection, glm::vec3(0.0, 0.0, -3.0));
    projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- ���� ��ȯ �� ����  
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]); 
}
GLvoid View() { 
    glUseProgram(shaderID); 
    view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    view = view * camera;
    viewLocation = glGetUniformLocation(shaderID, "viewTransform");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}

GLvoid Floor() { 
    View();
    Pro();
    glUseProgram(shaderID); 
    glBindVertexArray(VAO[0]);  
    FloorModel = glm::mat4(1.0f); 
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");    
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(FloorModel));    
    glDrawArrays(GL_TRIANGLES, 0, 36);   
}
GLvoid CrainBody() {
    View();  
    Pro();   
    glUseProgram(shaderID);
    glBindVertexArray(VAO[1]);
    CrainModel = glm::mat4(1.0f);
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CrainModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);

}
GLvoid CrainBodyBottom() {
    View();
    Pro();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glUseProgram(shaderID);
    glBindVertexArray(VAO[2]);
    CrainModel = glm::mat4(1.0f);
   
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CrainModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
GLvoid CrainArm1() {  
    View(); 
    Pro(); 
    glUseProgram(shaderID);
    glBindVertexArray(VAO[3]); 
    CrainModel = glm::mat4(1.0f);
    
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CrainModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
} 
GLvoid CrainArm2() { 
    View();
    Pro();
    glUseProgram(shaderID);
    glBindVertexArray(VAO[4]); 
    CrainModel = glm::mat4(1.0f);
    
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CrainModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
} 

GLvoid TransFloor() {
    View();
    Pro();
    //a(); 
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    FloorModel = glm::mat4(1.0f);
    FloorModel = FloorModel * rAll; 
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(FloorModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
GLvoid TransCrainBody() {
    View();
    Pro(); 
    glUseProgram(shaderID);
    glBindVertexArray(VAO[1]);
    CrainModel = glm::mat4(1.0f);
    
    CrainModel = CrainModel *t*r*rAll;    
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CrainModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);

}
GLvoid TransCrainBodyBottom() {
    View();
    Pro();
    glUseProgram(shaderID);
    glBindVertexArray(VAO[2]);
    CrainModel = glm::mat4(1.0f);
    CrainModel = CrainModel*t * rAll;
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CrainModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
GLvoid TransCrainArm1() {
    View();
    Pro();
    glUseProgram(shaderID);
    glBindVertexArray(VAO[3]);
    CrainModel = glm::mat4(1.0f);
    CrainModel = CrainModel *t*r*ar1 * rAll;
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CrainModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
GLvoid TransCrainArm2() { 
    View();
    Pro();
    glUseProgram(shaderID);
    glBindVertexArray(VAO[4]);
    CrainModel = glm::mat4(1.0f);
   
    CrainModel = CrainModel* t*r*ar2 * rAll; 
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CrainModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
    glClearColor(0, 0, 0, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    if (s.current == '0') {
        Floor();
        CrainBodyBottom();
        CrainBody();
        CrainArm1();
        CrainArm2(); 
    }
    
    else {
        if (s.current == 'a' || s.current == 'A')
            TransFloor();
        else
            Floor(); 
        TransCrainBodyBottom();
        TransCrainBody();
        TransCrainArm1();
        TransCrainArm2();
    } 
    glutPostRedisplay();
    glutSwapBuffers();
}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� 
{
    glViewport(0, 0, w, h);
}

GLfloat dis = 0;   
GLfloat rAngle;   
GLfloat aAngle1;
GLfloat aAngle2; 
GLfloat AllAngle;
GLfloat cameAngle; 

GLvoid b(int value) {
    if (s.isb % 2 !=0) {
        t = glm::mat4(1.0f);  
        t = glm::translate(t,glm::vec3(0,0,dis));  
        dis += 0.01;
        glutTimerFunc(200, b, 0);  
    }
} 
GLvoid B(int value) { 
    if ( s.isB % 2 != 0) { 
        t = glm::mat4(1.0f);
        t = glm::translate(t, glm::vec3(0, 0, dis));
        dis -= 0.01; 
        glutTimerFunc(200, B, 0); 
    }
}  
GLvoid m(int value) {
    if (s.ism % 2 != 0) {
        angle = glm::radians(rAngle); 
        r = glm::mat4(1.0f); 
        r = glm::rotate(r, angle, glm::vec3(0,1,0)); 
        rAngle += 10;  
        glutTimerFunc(50, m, 0); 
    }
}

GLvoid M(int value) {
    if (s.isM % 2 != 0) { 
        angle = glm::radians(rAngle);
        r = glm::mat4(1.0f);  
        r = glm::rotate(r, angle, glm::vec3(0, 1, 0));  
        rAngle -= 10;  
        glutTimerFunc(50, M, 0); 
    }
}

int limit1 = 0;
int limit2 = 0;
GLvoid tt(int value) {   
    if (s.ist % 2 != 0) {
        if (limit1 == 0) {
            angle = glm::radians(aAngle1);
            ar1 = glm::mat4(1.0f);
            ar1 = glm::translate(ar1, glm::vec3(-0.05, 0.7, 0.05));
            ar1 = glm::rotate(ar1, angle, glm::vec3(0, 0, 1));
            ar1 = glm::translate(ar1,glm::vec3(0.05, -0.7, -0.05));
            aAngle1 -= 10;
            if (aAngle1 <= -90) {
                limit1 = 1;
            }
        }
        if (limit1 == 1) { 
            angle = glm::radians(aAngle1);
            ar1 = glm::mat4(1.0f);
            ar1 = glm::translate(ar1, glm::vec3(-0.05, 0.7, 0.05));
            ar1 = glm::rotate(ar1, angle, glm::vec3(0, 0, 1));
            ar1 = glm::translate(ar1, glm::vec3(0.05, -0.7, -0.05)); 
            aAngle1 += 10; 
            if (aAngle1 >= 90) { 
                limit1 = 0; 
            }
        }
        if (limit2 == 0) {
            angle = glm::radians(aAngle2);
            ar2 = glm::mat4(1.0f);
            ar2 = glm::translate(ar2, glm::vec3(0.03, 0.7, 0.03));
            ar2 = glm::rotate(ar2, angle, glm::vec3(0, 0, 1));
            ar2 = glm::translate(ar2, glm::vec3(-0.03, -0.7, -0.03));
            aAngle2 += 10;
            if (aAngle2 >= 90) {
                limit2 = 1;
            }
        }
        if (limit2 == 1) {
            angle = glm::radians(aAngle2);
            ar2 = glm::mat4(1.0f);
            ar2 = glm::translate(ar2, glm::vec3(0.03, 0.7, 0.03));
            ar2 = glm::rotate(ar2, angle, glm::vec3(0, 0, 1));
            ar2 = glm::translate(ar2, glm::vec3(-0.03, -0.7, -0.03));
            aAngle2 -= 10;
            if (aAngle2 <= -90) { 
                limit2 = 0;
            }
        }
        glutTimerFunc(50, tt, 0); 
    } 
}

GLvoid Y(int value) {  
    //ī�޶��ڽ��� �߽�����  ȸ�� 
    if (s.isy % 2 != 0) {
        angle = glm::radians(cameAngle);
        camera = glm::mat4(1.0f);
        camera = glm::translate(camera, glm::vec3(0, 1, 1)); 
        camera = glm::rotate(camera, angle, glm::vec3(0, 1, 0));
        camera = glm::translate(camera, glm::vec3(0,-1,-1));
        cameAngle += 5;
        glutTimerFunc(100, Y, 0);  
    }
   
}

GLfloat theta;
GLvoid R(int value) {
    //ī�޶� ȸ�� 
    if (s.isr % 2 != 0) {
        angle = glm::radians(theta); 
        camera = glm::mat4(1.0f); 
        camera = glm::rotate(camera, angle, glm::vec3(0, 1, 0)); 

        theta += 5;
        glutTimerFunc(100, R, 0);
    }
}
GLvoid RR(int value) {
    //ī�޶� ȸ�� 
    if (s.isR % 2 != 0) {
        angle = glm::radians(theta);
        camera = glm::mat4(1.0f);
        camera = glm::rotate(camera, angle, glm::vec3(0, 1, 0));

        theta -= 5; 
        glutTimerFunc(100, RR, 0); 
    }
}

GLvoid a(int value) {
    if (s.isa % 2 != 0) {
        angle = glm::radians(AllAngle); 
        rAll = glm::mat4(1.0);
        rAll = glm::rotate(rAll,angle,glm::vec3(0,1,0)); 
        AllAngle += 5;
        glutTimerFunc(100, a, 0);
    }
}
GLvoid A(int value) {
    if (s.isA % 2 != 0) {
        angle = glm::radians(AllAngle); 
        rAll = glm::mat4(1.0); 
        rAll = glm::rotate(rAll, angle, glm::vec3(0, 1, 0));  
        AllAngle -= 5; 
        glutTimerFunc(100, A, 0); 
    }
}

GLvoid myKeyBoard(unsigned char key, int x, int y) {
    switch (key) {
    case'b':
        s.current = 'b'; 
        s.isb += 1;
        if (s.isb % 2 != 0) { 
            glutTimerFunc(200, b, 0); 
        }
        break;
    case 'B': 
        s.current = 'B';
        s.isB += 1;
        if (s.isB % 2 != 0) {
            glutTimerFunc(200, B, 0); 
        }
        break;
    case 'm':
        s.current = 'm';
        s.ism += 1;
        if (s.ism % 2 != 0) {
            glutTimerFunc(50,m,0); 
        }
        break; 
    case 'M': 
        s.current = 'M';
        s.isM += 1;
        if (s.isM % 2 != 0) {
            glutTimerFunc(50, M, 0);  
        }
        break;
    case 't': 
        s.current = 't';
        s.ist += 1;
        if (s.ist % 2 != 0) {
            glutTimerFunc(50, tt, 0); 
        }
        break; 
    //ī�޶� ��ȯ 
    case 'x':
        cx += 0.03;   
        cameraPos = glm::vec3(cx, cy, cz); 
        cameraDirection = glm::vec3(cx, 0.3f, 0.0f);
        break;
    case 'X':
        cx -= 0.03; 
        cameraPos = glm::vec3(cx, cy, cz);
        cameraDirection = glm::vec3(cx, 0.3f, 0.0f);
        break;
    case 'z':
        cz += 0.03;
        cameraPos = glm::vec3(cx, cy, cz);
        cameraDirection = glm::vec3(cx, 0.3f, 0.0f);
        break;
    case 'Z':
        cz -= 0.03; 
        cameraPos = glm::vec3(cx, cy, cz); 
        cameraDirection = glm::vec3(cx, 0.3f, 0.0f); 
        break;
    case 'y':
        //ī�޶� ���� y������ ȸ��  
        s.isy += 1;
        if (s.isy % 2 != 0) {
            glutTimerFunc(100, Y, 0);  
        }
        break;
    case 'r':
        s.current = 'r';
        s.isr += 1;
        if (s.isr % 2 != 0) {
            glutTimerFunc(100, R, 0);
        }
        break;
    case 'R':
        s.current = 'R';
        s.isR += 1;
        if (s.isR % 2 != 0) {
            glutTimerFunc(100, RR, 0); 
        }
        break;
    case 'a':
        s.current = 'a';
        s.isa += 1;
        if (s.isa % 2 != 0) {
            glutTimerFunc(100, a, 0);
        }
        break;
    case 'A':
        s.current = 'A'; 
        s.isA += 1;
        if (s.isA % 2 != 0) {
            glutTimerFunc(100, A, 0); 
        }
        break;
    case 's':
        //��� ������ ���߱�
        s.current = 's';
        s.isB = 0;
        s.isb = 0;
        s.isM = 0;
        s.ism = 0;
        s.ist = 0;
        s.isy = 0;
        s.isr = 0;
        s.isR = 0;
        s.isa = 0;
        s.isA = 0;
        break; 
    case 'c':
        //��� ������ �ʱ�ȭ  
        t = glm::mat4(1.0f); //�̵� ���  
        r = glm::mat4(1.0f); //ȸ�� ���    
        ar1 = glm::mat4(1.0f); //�� ȸ�� ���     
        ar2 = glm::mat4(1.0f); //�� ȸ�� ���  
        camera = glm::mat4(1.0f); //ī�޶� ȸ�� ��� 
        s.current = '0'; 
        cx = 0.0f;
        cy = 1.0f; 
        cz = 1.0f;
        cameraPos = glm::vec3(cx, cy, cz);
        cameraDirection = glm::vec3(cx, 0.3f, 0.0f);
        limit1 = 0;
        limit2 = 0;
        dis = 0;
        rAngle =0;
        aAngle1 =0;
        aAngle2 =0; 
        theta = 0; 
        AllAngle =0; 
        cameAngle = 0; 
        s.isB = 0;
        s.isb = 0;
        s.isM = 0;
        s.ism = 0;
        s.ist = 0;
        s.isy = 0; 
        s.isr = 0;
        s.isR = 0;
        s.isa = 0; 
        s.isA = 0; 
        break;
   
    case 'Q': //����  
        cout << "- Program End - " << endl;
        exit(-1);
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Practice19"); 

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }
    else
        cout << "GLEW Initialized\n";

    make_vertexShaders();
    make_fragmentShaders();

    shaderID = make_shaderProgram();

    InitBuffer();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(myKeyBoard);
    glutMainLoop();
}