#include "obj_shaderRead.h" 

int num_Triangle;
objReader obj;
GLuint VAO[3], VBO_pos[3], VBO_normal[3], VBO_color[3];

//------------------------------------
glm::mat4 CubeModel = glm::mat4(1.0f);
glm::mat4 MiroModel = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 camera = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
//-----------------------------------  

float cx = 0.0;
float cy = 0.7;
float cz = 0.2;

float c2x = 0.0;
float c2y = 0.0;
float c2z = 0.0;

glm::vec3 cameraPos = glm::vec3(cx, cy, cz);
glm::vec3 cameraDirection = glm::vec3(c2x, c2y, c2z);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat lightX = 2.0, lightY = 2.0, lightZ = 0.0;
GLfloat lightR = 1.0, lightG = 1.0, lightB = 1.0;
GLfloat BoxX = 1.5, BoxY = 1.5, BoxZ = 1.0;
//------------------------------------ 
glm::mat4 TR = glm::mat4(1.0f);
//----------------------------------------------
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
    for (int i = 0; i < h; i++) {
        MDataArr[i] = new loca[w];
    }

    srand((unsigned int)time(NULL));
    for (int i = 0; i < 20; i++) {
        for (int p = 0; p < 20; p++) { 
            MDataArr[i][p].is = 0;  
            //크기 스케일  
            MDataArr[i][p].sx = 1.0 / 20;
            MDataArr[i][p].sy = 0.3; 
            MDataArr[i][p].sz = 1.0 / 20;

            //위치 이동하기  
            MDataArr[i][p].x = (-0.5 + (1.0 / w) / 2) + ((1.0 / w) * p);
            MDataArr[i][p].y = 0;
            MDataArr[i][p].z = (-0.5 + (1.0 / h) / 2) + ((1.0 / h) * i);

            //------------------------------------------------------------- 


            //위치 계산하기 
            MDataArr[i][p].lx = (-0.5 + ((1.0 / 20) * p)); //왼     
            MDataArr[i][p].rx = (-0.5 + ((1.0 / 20) * p)) + (1.0 / 20); //오    
            MDataArr[i][p].uz = (-0.5 + ((1.0 / 20) * i)) + (1.0 / 20); //양의 방향쪽 z      
            MDataArr[i][p].dz = (-0.5 + ((1.0 / 20) * i)); //음의 방향쪽 z   

            if (MDataArr[i][p].lx + (1.0 / w) == 0) {
                MDataArr[i][p].rx = 0;
            }
            if (MDataArr[i][p].dz + (1.0 / h) == 0) {
                MDataArr[i][p].uz = 0;
            }
        }
    }
} 
void MakeMiro() {
    for (int i = 1; i < 6; ++i)
        MDataArr[0][i].is = 1;
    MDataArr[0][9].is = 1;
    MDataArr[0][11].is = 1;
    MDataArr[0][12].is = 1;
    MDataArr[0][14].is = 1;
    MDataArr[0][16].is = 1;

    MDataArr[1][1].is = 1;
    MDataArr[1][3].is = 1;
    MDataArr[1][5].is = 1;
    MDataArr[1][11].is = 1;
    for(int i = 12; i<= 18; i+=2)
        MDataArr[1][i].is = 1; 

    for (int i = 1; i <= 7; i += 2) {
        MDataArr[2][i].is = 1;
    }
    MDataArr[2][6].is = 1;
    MDataArr[2][14].is = 1;
    MDataArr[2][16].is = 1;
    MDataArr[2][18].is = 1;

    for (int i = 3; i < 13; i += 2) {
        MDataArr[3][i].is = 1;
    }
    MDataArr[3][12].is = 1;
    MDataArr[3][14].is = 1;
    MDataArr[3][18].is = 1;

    MDataArr[4][0].is = 1;
    MDataArr[4][1].is = 1;
    MDataArr[4][5].is = 1;
    MDataArr[4][9].is = 1;
    MDataArr[4][11].is = 1;
    MDataArr[4][18].is = 1;

    for (int i = 1; i <= 13; i += 2) { 
        MDataArr[5][i].is = 1;
    }
    MDataArr[5][5].is = 0;
    for (int i = 12; i <= 18; i += 2) { 
        MDataArr[5][i].is = 1;
    }

    for(int i = 1; i<=9; i+=2)
        MDataArr[6][i].is = 1; 
    MDataArr[6][5].is = 0;
    MDataArr[6][16].is = 1;
    MDataArr[6][18].is = 1;
    MDataArr[6][19].is = 1;

    for (int i = 0; i <= 16; ++i) {
        MDataArr[7][i].is = 1;
    }
    MDataArr[7][10].is = 0;
    MDataArr[7][12].is = 0;

    MDataArr[8][11].is = 1;
    MDataArr[8][13].is = 1;

    for (int i = 3; i <= 19; ++i) {
        MDataArr[9][i].is = 1;
    }
    MDataArr[9][12].is = 0;
    MDataArr[9][14].is = 0;
    MDataArr[9][15].is = 0;

    MDataArr[10][0].is = 1;
    MDataArr[10][1].is = 1; 
    MDataArr[10][6].is = 1;
    MDataArr[10][11].is = 1;
    MDataArr[10][16].is = 1;

    MDataArr[11][3].is = 1;
    MDataArr[11][8].is = 1;
    MDataArr[11][11].is = 1;
    MDataArr[11][13].is = 1;
    MDataArr[11][16].is = 1;
    MDataArr[11][17].is = 1;
    MDataArr[11][18].is = 1;

    for (int i = 0; i <= 8; ++i)
        MDataArr[12][i].is = 1;
    MDataArr[12][1].is = 0; 
    MDataArr[12][11].is = 1;
    MDataArr[12][13].is = 1;
    MDataArr[12][18].is = 1;

    MDataArr[13][3].is = 1;
    MDataArr[13][13].is = 1;
    MDataArr[13][15].is = 1;
    MDataArr[13][18].is = 1;

    MDataArr[14][0].is = 1;
    MDataArr[14][1].is = 1;
    MDataArr[14][3].is = 1;
    MDataArr[14][6].is = 1;
    MDataArr[14][9].is = 1;
    MDataArr[14][11].is = 1;
    MDataArr[14][15].is = 1;
    MDataArr[14][18].is = 1;

    for (int i = 1; i <= 18; ++i) {
        MDataArr[15][i].is = 1; 
    }
    MDataArr[15][4].is = 0; 
    MDataArr[15][5].is = 0;
    MDataArr[15][7].is = 0;
    MDataArr[15][8].is = 0;
    MDataArr[15][10].is = 0;
    MDataArr[15][12].is = 0;
    MDataArr[15][16].is = 0;
  

    MDataArr[16][6].is = 1;
    for (int i = 5; i <= 15; i += 2) {
        MDataArr[16][i].is = 1;
    }
    MDataArr[17][1].is = 1;
    MDataArr[17][9].is = 1;
    MDataArr[17][19].is = 1;

    for (int i = 0; i <= 19; ++i) {
        MDataArr[18][i].is = 1; 
    }
    MDataArr[18][6].is = 0; 
    MDataArr[18][7].is = 0;
    MDataArr[18][17].is = 0;

    MDataArr[19][2].is = 1;
    MDataArr[19][3].is = 1;
    MDataArr[19][12].is = 1;
    MDataArr[19][13].is = 1; 

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
    //원근 투영 ver 
    glUseProgram(shaderID);
    projection = glm::mat4(1.0f);
    Angle = glm::radians(45.0f);
    projection = glm::perspective(Angle, (float)800 / (float)600, 1.0f, 50.0f);
    projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2));
    projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정  
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
            if (MDataArr[i][p].is == 1) {  
                Mountins(i, p); 
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
    MakeMiro();  
    make_vertexShaders();
    make_fragmentShaders();

    InitBuffer();

    shaderID = make_shaderProgram();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutMainLoop();
}
