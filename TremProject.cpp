#include "obj_shaderRead.h" 

int num_Triangle;
objReader obj;
GLuint VAO[3], VBO_pos[3], VBO_normal[3], VBO_color[3];

//------------------------------------
glm::mat4 CubeModel = glm::mat4(1.0f);
glm::mat4 MiroModel = glm::mat4(1.0f);
glm::mat4 MiroModel2 = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 camera = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
//-----------------------------------  

float cx = 0.0;
float cy = 0.7;
float cz = 0.3;

float c2x = 0.0;
float c2y = 0.0;
float c2z = 0.0;

float Angle = 0.0f;

unsigned int modelLocation;
unsigned int projectionLocation;
unsigned int viewLocation;
unsigned int colorLocation;  //���� ����  

unsigned int lightColorLocation;
unsigned int lightPosLocation;
unsigned int ViewPosLocation;
unsigned int objColorLocation;
unsigned int normalLocation;

glm::vec3 cameraPos = glm::vec3(cx, cy, cz);
glm::vec3 cameraDirection = glm::vec3(c2x, c2y, c2z);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat lightX = 2.0, lightY = 2.0, lightZ = 0.0;
GLfloat lightR = 1.0, lightG = 1.0, lightB = 1.0;
GLfloat BoxX = 1.5, BoxY = 1.5, BoxZ = 1.0;
//------------------------------------ 
glm::mat4 TR = glm::mat4(1.0f);
glm::mat4 TS = glm::mat4(1.0f);
//----------------------------------------------
struct loca {
    //ť�갡 �̵��� ��ġ 
    float x{};
    float y{};
    float z{};

    //ť�갡 �����ϵ� ũ��   
    float sx{};
    float sy{};
    float sz{};

    //�׷������� ���� 
    int is{};

    //�����ϰ� �ö󰥰��� ���������� ���ϴ� ���� 
    int rnd{};

    //���� x,z�� ��ġ 
    float lx{};
    float rx{};
    float dz{};
    float uz{};
};
loca** MDataArr = nullptr;
loca** MDataArr2 = nullptr;
//-------------------------------------------------
GLuint Stage1[20][20] = { 0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,0,0,0,
                          0,1,1,1,1,1,0,0,0,0,0,1,1,0,1,0,1,0,1,0,
                          0,1,0,1,0,1,1,1,0,1,0,0,0,0,1,0,1,0,1,0,
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
                          1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,1,1,
                          0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0 };

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
// Ÿ�ϸ� ��ġ�� ����
//-------------------------------------------------
GLvoid InitBuffer() {
    num_Triangle = obj.loadObj("Cube.obj");
    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO_pos);
    glGenBuffers(3, VBO_normal);
    glGenBuffers(3, VBO_color);

    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos[0]);
    glBufferData(GL_ARRAY_BUFFER, obj.outvertex.size() * sizeof(glm::vec3), &obj.outvertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_normal[0]);
    glBufferData(GL_ARRAY_BUFFER, obj.outnormal.size() * sizeof(glm::vec3), &obj.outnormal[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
}





void UserFunc() {
    int w = 20;
    int h = 20;

    MDataArr = new loca * [h];
    MDataArr2 = new loca * [h];

    for (int i = 0; i < h; i++) {
        MDataArr[i] = new loca[w];
    }
    for (int i = 0; i < h; i++) {
        MDataArr2[i] = new loca[w];
    }

    srand((unsigned int)time(NULL));
    for (int i = 0; i < 20; i++) {
        for (int p = 0; p < 20; p++) {
            MDataArr[i][p].is = 0;
            //ũ�� ������  
            MDataArr[i][p].sx = 1.0 / 20;
            MDataArr[i][p].sy = 0.3;
            MDataArr[i][p].sz = 1.0 / 20;

            //��ġ �̵��ϱ�  
            MDataArr[i][p].x = (-0.5 + (1.0 / w) / 2) + ((1.0 / w) * p);
            MDataArr[i][p].y = 0;
            MDataArr[i][p].z = (-0.5 + (1.0 / h) / 2) + ((1.0 / h) * i);

            //------------------------------------------------------------- 


            //��ġ ����ϱ� 
            MDataArr[i][p].lx = (-0.5 + ((1.0 / 20) * p)); //��     
            MDataArr[i][p].rx = (-0.5 + ((1.0 / 20) * p)) + (1.0 / 20); //��    
            MDataArr[i][p].uz = (-0.5 + ((1.0 / 20) * i)) + (1.0 / 20); //���� ������ z      
            MDataArr[i][p].dz = (-0.5 + ((1.0 / 20) * i)); //���� ������ z   

            if (MDataArr[i][p].lx + (1.0 / w) == 0) {
                MDataArr[i][p].rx = 0;
            }
            if (MDataArr[i][p].dz + (1.0 / h) == 0) {
                MDataArr[i][p].uz = 0;
            }
            //--------------------------------------------------------------------

            MDataArr2[i][p].is = 0;
            //ũ�� ������  
            MDataArr2[i][p].sx = 1.0 / 20;
            MDataArr2[i][p].sy = 0.3;
            MDataArr2[i][p].sz = 1.0 / 20;

            //��ġ �̵��ϱ�  
            MDataArr2[i][p].x = (-0.5 + (1.0 / w) / 2) + ((1.0 / w) * p);
            MDataArr2[i][p].y = 0;
            MDataArr2[i][p].z = (-0.5 + (1.0 / h) / 2) + ((1.0 / h) * i);

            //------------------------------------------------------------- 


            //��ġ ����ϱ� 
            MDataArr2[i][p].lx = (-0.5 + ((1.0 / 20) * p)); //��     
            MDataArr2[i][p].rx = (-0.5 + ((1.0 / 20) * p)) + (1.0 / 20); //��    
            MDataArr2[i][p].uz = (-0.5 + ((1.0 / 20) * i)) + (1.0 / 20); //���� ������ z      
            MDataArr2[i][p].dz = (-0.5 + ((1.0 / 20) * i)); //���� ������ z   

            if (MDataArr2[i][p].lx + (1.0 / w) == 0) {
                MDataArr2[i][p].rx = 0;
            }
            if (MDataArr2[i][p].dz + (1.0 / h) == 0) {
                MDataArr2[i][p].uz = 0;
            }

        }
    }
}
GLvoid InitLight() {
    lightColorLocation = glGetUniformLocation(shaderID, "lightColor");
    objColorLocation = glGetUniformLocation(shaderID, "objectColor");
    lightPosLocation = glGetUniformLocation(shaderID, "lightPos");
    ViewPosLocation = glGetUniformLocation(shaderID, "viewPos");

    glUniform3f(lightColorLocation, lightR, lightG, lightB); //light Color     
    glUniform3f(lightPosLocation, lightX, lightY, lightZ);
    glUniform3f(ViewPosLocation, cx, cy, cz);
}
GLvoid Pro() {
    //���� ���� ver 
    glUseProgram(shaderID);
    projection = glm::mat4(1.0f);
    Angle = glm::radians(45.0f);
    projection = glm::perspective(Angle, (float)800 / (float)600, 1.0f, 50.0f);
    projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2));
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
GLvoid Mountins(int i, int p) {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation, 0.3, 0.3, 0.3);

    MiroModel = glm::mat4(1.0f);
    MiroModel = glm::translate(MiroModel, glm::vec3(MDataArr[i][p].x, 0.3, MDataArr[i][p].z));
    MiroModel = glm::scale(MiroModel, glm::vec3(MDataArr[i][p].sx, MDataArr[i][p].sy, MDataArr[i][p].sz));
    MiroModel = glm::translate(MiroModel, glm::vec3(0, -0.3, 0));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MiroModel));
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
}
GLvoid Mountins2(int i, int p) {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation, 0.3, 0.3, 0.3);

    MiroModel2 = glm::mat4(1.0f);
    MiroModel2 = glm::translate(MiroModel2, glm::vec3(MDataArr2[i][p].x, 0.7, MDataArr2[i][p].z)); // �� ���� �ٲ��� stage2 �ٴ��� ��ġ (2��)
    MiroModel2 = glm::scale(MiroModel2, glm::vec3(MDataArr2[i][p].sx, MDataArr2[i][p].sy, MDataArr2[i][p].sz));
    MiroModel2 = glm::translate(MiroModel2, glm::vec3(0, -0.3, 0));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MiroModel2));
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
}
GLvoid Floor() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation, 0.5, 0.5, 0.5);

    CubeModel = glm::mat4(1.0f);
    CubeModel = glm::scale(CubeModel, glm::vec3(1, 0.1, 1));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CubeModel));
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
}
GLvoid Floor2() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation, 0.5, 0.5, 0.5);

    CubeModel = glm::mat4(1.0f);
    CubeModel = glm::scale(CubeModel, glm::vec3(1, 0.1, 1));
    CubeModel = glm::translate(CubeModel, glm::vec3(0.0, 4.1, 0)); // y�� 4.1�� ���� (2��)

    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CubeModel));
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
}
GLvoid drawScene()
{
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    Pro();
    View();

    Floor();
    for (int i = 0; i < 20; i++) {
        for (int p = 0; p < 20; p++) {
            if (Stage1[i][p] == 1) {
                Mountins(i, p);
            }
        }
    }
    Floor2();
    for (int i = 0; i < 20; i++) {
        for (int p = 0; p < 20; p++) {
            if (Stage2[i][p] == 1) {
                Mountins2(i, p);
            }
        }
    }
    glutPostRedisplay();
    glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1500, 1500);
    glutCreateWindow("TermProject");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }
    else
        cout << "GLEW Initialized\n";

    UserFunc();
    //MakeMiro();
    make_vertexShaders();
    make_fragmentShaders();

    InitBuffer();

    shaderID = make_shaderProgram();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}
