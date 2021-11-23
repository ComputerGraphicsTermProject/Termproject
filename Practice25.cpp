#include "obj_shaderRead.h"
GLuint VAO[3], VBO_pos[3], VBO_normal[3], VBO_color[3];
int num_Triangle;
objReader obj;
glm::mat4 SphereModel = glm::mat4(1.0f); 
glm::mat4 LightModel = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
//-----------------------------------------------
unsigned int modelLocation;
unsigned int projectionLocation;
unsigned int viewLocation;
unsigned int colorLocation;  //모델의 색상 

unsigned int lightColorLocation;
unsigned int lightPosLocation;
unsigned int ViewPosLocation;
unsigned int objColorLocation;
unsigned int normalLocation;
//-------------------------------------------------
glm::vec3 cameraPos = glm::vec3(0.0f, 0.3f, 0.9f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//--------------------------------------------------
glm::mat4 TS = glm::mat4(1.0f);  
glm::mat4 TT = glm::mat4(1.0f);
//--------------------------------------------------
GLfloat lightX = -2.0, lightY = 0.0, lightZ = -2.0;      
GLfloat BoxX =1.0, BoxY = 0.0, BoxZ =1.0;   
int current = 0; 
GLfloat A;
float vertices[] = { //--- 버텍스 속성: 좌표값(FragPos), 노말값 (Normal)
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};  
GLfloat Data[36][6] = {
    {-0.5,0.5,0.5,0.1,0.1,0.1},
    {-0.5,-0.5,0.5,0.1,0.1,0.1},
    {0.5,-0.5,0.5,0.1,0.1,0.1},

    {-0.5,0.5,0.5,0.1,0.1,0.1},
    {0.5,-0.5,0.5,0.1,0.1,0.1},
    {0.5,0.5,0.5,0.1,0.1,0.1}, //앞면 

    {-0.5,0.5,-0.5,0.1,0.1,0.1},
    {0.5,-0.5,-0.5,0.1,0.1,0.1},
    {-0.5,-0.5,-0.5,0.1,0.1,0.1},

    {-0.5,0.5,-0.5,0.1,0.1,0.1},
    {0.5,0.5,-0.5,0.1,0.1,0.1},
    {0.5,-0.5,-0.5,0.1,0.1,0.1}, //뒷면  

    {-0.5,0.5,-0.5,0.1,0.1,0.1},
    {-0.5,-0.5,-0.5,0.1,0.1,0.1},
    {-0.5,0.5,0.5,0.1,0.1,0.1},

    {-0.5,0.5,0.5,0.1,0.1,0.1},
    {-0.5,-0.5,-0.5,0.1,0.1,0.1},
    {-0.5,-0.5,0.5,0.1,0.1,0.1}, //왼쪽 옆면

    {0.5,0.5,0.5,0.1,0.1,0.1},
    {0.5,-0.5,0.5,0.1,0.1,0.1},
    {0.5,-0.5,-0.5,0.1,0.1,0.1},

    {0.5,0.5,0.5,0.1,0.1,0.1},
    {0.5,-0.5,-0.5,0.1,0.1,0.1},
    {0.5,0.5,-0.5,0.1,0.1,0.1},  //오른쪽 옆면

    {-0.5,0.5,-0.5,0.1,0.1,0.1},
    {-0.5,0.5,0.5,0.1,0.1,0.1},
    {0.5,0.5,0.5,0.1,0.1,0.1},

    {-0.5,0.5,-0.5,0.1,0.1,0.1},
    {0.5,0.5,0.5,0.1,0.1,0.1},
    {0.5,0.5,-0.5,0.1,0.1,0.1}, //윗면

    {-0.5,-0.5,0.5,0.1,0.1,0.1},
    {-0.5,-0.5,-0.5,0.1,0.1,0.1},
    {0.5,-0.5,0.5,0.1,0.1,0.1},

    {0.5,-0.5,0.5,0.1,0.1,0.1},
    {-0.5,-0.5,-0.5,0.1,0.1,0.1},
    {0.5,-0.5,-0.5,0.1,0.1,0.1} //밑면 
};  
GLuint VBO2, VAO2; 
GLuint VAO3, VBO3; 
GLvoid InitBuffer() { 
    num_Triangle = obj.loadObj_normalize_center("Sphere.obj");  
    // 5.1. VAO 객체 생성 및 바인딩 
    glGenVertexArrays(3, VAO); 
    glGenBuffers(3, VBO_pos);
    glGenBuffers(3, VBO_normal);
    glGenBuffers(3, VBO_color);

    // 2 triangles for quad floor
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

    // 조명 및 큐브 
    glGenVertexArrays(1, &VAO2); 
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2); 

    glBufferData(GL_ARRAY_BUFFER, (36 * 6) * sizeof(GLfloat), Data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
    glEnableVertexAttribArray(1);   
}

GLvoid Pro() {
    //원근 투영 ver 
    glUseProgram(shaderID);
    projection = glm::mat4(1.0f);
    A = glm::radians(45.0f);  
    projection = glm::perspective(A, (float)800 / (float)600, 1.0f, 50.0f);
    projection = glm::translate(projection, glm::vec3(0.0, 0.0, -4.0)); 
    projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정  
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);


}
GLvoid View() {
    glUseProgram(shaderID);
    view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    viewLocation = glGetUniformLocation(shaderID, "viewTransform");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}

GLvoid InitLight() { 
    glBindVertexArray(VAO2);
    lightColorLocation = glGetUniformLocation(shaderID, "lightColor");
    objColorLocation = glGetUniformLocation(shaderID, "objectColor");
    lightPosLocation = glGetUniformLocation(shaderID, "lightPos");
    ViewPosLocation = glGetUniformLocation(shaderID, "viewPos");
    glUseProgram(shaderID);
    glUniform3f(lightPosLocation, lightX, lightY, lightZ); 
    glUniform3f(ViewPosLocation, lightX, lightY, lightZ); 
} 
GLvoid Sphere1() { 
    Pro(); 
    View(); 
    InitLight(); 
    glUniform3f(objColorLocation, 1, 0, 0); //obj Color    
     
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    SphereModel = glm::mat4(1.0f);  
    TS = glm::mat4(1.0f);  
    TT = glm::mat4(1.0f); 
    TT = glm::translate(TT, glm::vec3(0.7, 0, 0));   
    SphereModel = SphereModel * TT; 
    TS = glm::scale(TS, glm::vec3(0.5,0.5,0.5));  
    SphereModel = SphereModel * TS; 

    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(SphereModel));  
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle); 
} 
GLvoid Sphere2() { 
    Pro();
    View();
    InitLight();  
    glUniform3f(objColorLocation, 0,1,0 ); //obj Color      
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]); 
    SphereModel = glm::mat4(1.0f);  
    TS = glm::mat4(1.0f);
    TT = glm::mat4(1.0f);
    TT = glm::translate(TT, glm::vec3(-0.5, 0, 0));
    SphereModel = SphereModel * TT;
    TS = glm::scale(TS, glm::vec3(0.3, 0.3, 0.3));
    SphereModel = SphereModel * TS;
   

    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(SphereModel));
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
}
GLvoid Sphere3() { 
    Pro();
    View();
    InitLight();
    glUniform3f(objColorLocation, 0, 0, 1); //obj Color         

    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    SphereModel = glm::mat4(1.0f);
    TS = glm::mat4(1.0f);
    TT = glm::mat4(1.0f); 
    TT = glm::translate(TT, glm::vec3(-1.5, 0, 0));  
    SphereModel = SphereModel * TT;
    TS = glm::scale(TS, glm::vec3(0.2,0.2,0.2));  
    SphereModel = SphereModel * TS;
   

    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(SphereModel));
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
}
GLvoid LightOBJ1() {
    Pro();
    View();
    InitLight(); 
    glUniform3f(objColorLocation, 1,1,1); //obj Color        
    glUseProgram(shaderID); 
    glBindVertexArray(VAO2);           
    LightModel = glm::mat4(1.0f);  
    TS = glm::mat4(1.0f);
    TT = glm::mat4(1.0f);
    TT = glm::translate(TT, glm::vec3(BoxX, BoxY, BoxZ));  
    LightModel = LightModel * TT;  
    TS = glm::scale(TS, glm::vec3(0.3,0.3,0.3)); 
    LightModel = LightModel * TS;

    modelLocation = glGetUniformLocation(shaderID, "modelTransform"); 
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(LightModel)); 
    glDrawArrays(GL_TRIANGLES, 0, 36);   
}



int remote = 0;
GLvoid drawScene()
{
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    Pro();
    View();

    if (remote == 0)
        glUniform3f(lightColorLocation, 1.0, 1.0, 1.0); //light Color      
    else if (remote == 1)
        glUniform3f(lightColorLocation, 0.5, 0.5, 0.5); //light Color       
    else if (remote == 2)
        glUniform3f(lightColorLocation, 0.3, 0.3, 0.3); //light Color        

    Sphere1();
    LightOBJ1();
    Sphere2();
    Sphere3();

    glutPostRedisplay();
    glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

int isRotate = 0;
int i = 0;  

GLvoid Rotate(int value) {
    if (isRotate == 1) { 
        lightX = sin(glm::radians((float)i)) * 5.0; 
        lightZ = cos(glm::radians((float)i)) * 5.0; 

        BoxX = sin(glm::radians((float)i)) * 2.0;
        BoxZ = cos(glm::radians((float)i)) * 2.0;   
        i += 3;
        if (i > 360)
            i = 0; 
        glutTimerFunc(100, Rotate, 0);  
    }
} 
 
GLvoid myKeyBoard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        remote = 0;  
        break; 
    case 'b':
        remote = 1; 
        break; 
    case 'c': 
        remote = 2;   
        break; 
    case 'r': 
        isRotate = 1;  
        glutTimerFunc(100,Rotate,0);
        break;
    case 'R':
        isRotate = 0; 
        break;
    case 'Q':
        puts("- Program End - ");
        exit(-1);
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Practice25");
     
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
    InitBuffer();
  
    shaderID = make_shaderProgram();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(myKeyBoard); 
    glutMainLoop();



}
