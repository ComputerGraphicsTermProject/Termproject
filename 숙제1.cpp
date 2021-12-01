#include "HW_1.h"  

void UserFunc() {
    w = 20;
    h = 20; 

    MDataArr = new loca * [h];
    for (int i = 0; i < h; i++) {
        MDataArr[i] = new loca[w];
    }

    srand((unsigned int)time(NULL));
    for (int i = 0; i < h; i++) {
        for (int p = 0; p < w; p++) {
            MDataArr[i][p].is = 1;
            //크기 스케일
            MDataArr[i][p].sx = 1.0 / w; 
            MDataArr[i][p].sy = (rand() % 2) + 1; 
            float rnd = rand() / (float)RAND_MAX; 
            MDataArr[i][p].sy = (MDataArr[i][p].sy) + rnd; 
            MDataArr[i][p].sz = 1.0 / h; 
            ////위치 이동하기 
            MDataArr[i][p].x = (-0.5 + (1.0 / w) / 2) + ((1.0 / w) * p); 
            MDataArr[i][p].y = 0; 
            MDataArr[i][p].z = (-0.5 + (1.0 / h) / 2) + ((1.0 / h) * i);
             
            //위치 계산하기 
            MDataArr[i][p].lx = (-0.5 + ((1.0 / w) * p)); //왼     
            MDataArr[i][p].rx = (-0.5 + ((1.0 / w) * p)) + (1.0 / w); //오    
            MDataArr[i][p].uz = (-0.5 + ((1.0 / h) * i)) + (1.0 / h); //양의 방향쪽 z      
            MDataArr[i][p].dz = (-0.5 + ((1.0 / h) * i)); //음의 방향쪽 z   

            if (MDataArr[i][p].lx + (1.0 / w) == 0) {
                MDataArr[i][p].rx = 0;
            }
            if (MDataArr[i][p].dz + (1.0 / h) == 0) {
                MDataArr[i][p].uz = 0;
            }
        }
    }
}
GLvoid Pro() {
    if (s.isO == 1) {
        glUseProgram(shaderID);
        projection = glm::mat4(1.0f);
        projection = glm::ortho(-1.5, 1.5, -1.5, 1.5, -1.5, 1.5); 
    }
    else if (s.isP == 1) {
        //원근 투영
        glUseProgram(shaderID);
        projection = glm::mat4(1.0f);
        angle = glm::radians(20.0f);
        projection = glm::perspective(angle, (float)800 / (float)800, 1.0f, 50.0f);
        projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));
    }
    else {
        glUseProgram(shaderID);
        projection = glm::mat4(1.0f);
        angle = glm::radians(20.0f);
        projection = glm::perspective(angle, (float)800 / (float)800, 1.0f, 50.0f);
        projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0));
    }
    projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정  
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}
GLvoid View() {
    glUseProgram(shaderID);
    view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
    view = view * cX * cY;
    viewLocation = glGetUniformLocation(shaderID, "viewTransform");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}
GLvoid Floor() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    CubeModel = glm::mat4(1.0f);
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CubeModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
GLvoid Mountins(int i, int p) { 
    glUseProgram(shaderID);
    glBindVertexArray(VAO[1]);
    MountinModel = glm::mat4(1.0f);
    MountinModel = glm::translate(MountinModel, glm::vec3(MDataArr[i][p].x, 0.5, MDataArr[i][p].z));
    MountinModel = glm::scale(MountinModel, glm::vec3(MDataArr[i][p].sx, MDataArr[i][p].sy, MDataArr[i][p].sz));
    MountinModel = glm::translate(MountinModel, glm::vec3(0, -0.5, 0));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MountinModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
} 
GLvoid PinkCharac() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[2]);
    CharacterModel = glm::mat4(1.0f);
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CharacterModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
} 
GLvoid TransPinkCharac() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[2]);
    CharacterModel = glm::mat4(1.0f);
    CharacterModel = CharacterModel * t;
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CharacterModel));
    glDrawArrays(GL_TRIANGLES, 0, 36);
} 

GLvoid Pro2() {
    glUseProgram(shaderID);
    projection2 = glm::mat4(1.0f);
    angle = glm::radians(20.0f);
    projection2 = glm::perspective(angle, 1.0f, 1.0f, 100.0f);
    projection2 = glm::translate(projection2, glm::vec3(0.0, 0.0, -5.0));
    projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정  
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection2[0][0]);
}
GLvoid View2() {
    glUseProgram(shaderID);
    view2 = glm::mat4(1.0f);
    view2 = glm::lookAt(cameraPos2, cameraDirection2, cameraUp2);
    viewLocation = glGetUniformLocation(shaderID, "viewTransform");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view2[0][0]);
}
//1인칭 시점 뷰,투영 
GLvoid ProOne() {
    glUseProgram(shaderID);
    projection = glm::mat4(1.0f);
    angle = glm::radians(20.0f);
    projection = glm::perspective(angle, (float)800 / (float)800, 1.0f, 50.0f);
    projection = glm::translate(projection, glm::vec3(0.0, 0.0, -1.0));

    projectionLocation = glGetUniformLocation(shaderID, "projectionTransform"); //--- 투영 변환 값 설정  
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}
GLvoid ViewOne() {
    glUseProgram(shaderID);
    view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPosOne, cameraDirectionOne, cameraUpOne);
    viewLocation = glGetUniformLocation(shaderID, "viewTransform");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
} 

GLvoid InitLightFloor() {
    lightColorLocation = glGetUniformLocation(shaderID, "lightColor");   
    objColorLocation = glGetUniformLocation(shaderID, "objectColor");   
    lightPosLocation = glGetUniformLocation(shaderID, "lightPos");   
    glUseProgram(shaderID);    
    glBindVertexArray(VAO2);    
    glUniform3f(objColorLocation, 0.1, 0.1, 0.1);           
    glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);         
    glUniform3f(lightPosLocation, 0.7,0.7,0.7);   
}
GLvoid InitLightMountin() {
    lightColorLocation = glGetUniformLocation(shaderID, "lightColor");
    objColorLocation = glGetUniformLocation(shaderID, "objectColor");
    lightPosLocation = glGetUniformLocation(shaderID, "lightPos");
    glUseProgram(shaderID);
    glBindVertexArray(VAO2);
    glUniform3f(objColorLocation, 0.6, 0.6, 0.6);
    glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
    glUniform3f(lightPosLocation, 0.7,0.7,0.7);
}
GLvoid InitLightCharac() {
    lightColorLocation = glGetUniformLocation(shaderID, "lightColor");
    objColorLocation = glGetUniformLocation(shaderID, "objectColor");
    lightPosLocation = glGetUniformLocation(shaderID, "lightPos");
    glUseProgram(shaderID);
    glBindVertexArray(VAO2);
    glUniform3f(objColorLocation, 1.0, 0.2, 0.2);
    glUniform3f(lightColorLocation, 1.0, 1.0, 1.0); 
    glUniform3f(lightPosLocation, 0.7,0.7,0.7);    
}
GLvoid Init() {
    //미로 만들 때 현재의 좌표 받아오는 변수 
    currenti = 0;
    currentp = 0;
    //캐릭터가 존재하는 배열의 범위 
    inCharaci = 0;
    inCharacp = 0;
    s.isO = 0;
    s.isP = 0;
    s.ism = 0;
    s.isx = 0;
    s.isX = 0;
    s.isy = 0;
    s.isY = 0;
    s.isr = 0;
    s.isv = 0;
    s.isplus = 0;
    s.isminus = 0;
    //횟수 제한 
    s.cntx = 0;
    s.cntX = 0;
    s.cnty = 0;
    s.cntY = 0;
    s.cntv = 0;
    //시점 변환 
    s.is1ver = 0;
    s.is3ver = 1;
    s.miniMap = 0;
    s.mode = '0';
    s.current = '0';
    Camangle = 0;
}

GLvoid m(int value) {
    //육면체들이 위 아래로 움직이고, 멈추기 (MDataArr[i][p].sy이 3을 넘어서면 다시 내려오고, 0보다 작으면 다시 올라오기)
    if (s.ism == 1) {
        for (int i = 0; i < h; i++) {
            for (int p = 0; p < w; p++) {
                //각 객체가 위아래로 움직이게 하기  
                if (MDataArr[i][p].rnd == 1) {
                    MDataArr[i][p].sy += up;
                    if (MDataArr[i][p].sy > 4) {
                        MDataArr[i][p].rnd = 0;
                    }
                }
                else if (MDataArr[i][p].rnd == 0) {
                    MDataArr[i][p].sy += down;
                    if (MDataArr[i][p].sy <= 0.1) {
                        MDataArr[i][p].rnd = 1;
                    }
                }
            }
        }
        glutTimerFunc(100, m, 0);
    }
}
GLvoid m_speedPlus(int value) {
    //육면체들이 위 아래로 움직이고, 멈추기 (MDataArr[i][p].sy이 3을 넘어서면 다시 내려오고, 0보다 작으면 다시 올라오기)
    if (s.isplus == 1) {
        for (int i = 0; i < h; i++) {
            for (int p = 0; p < w; p++) {
                //각 객체가 위아래로 움직이게 하기  
                if (MDataArr[i][p].rnd == 1) {
                    MDataArr[i][p].sy += up;
                    if (MDataArr[i][p].sy > 4) {
                        MDataArr[i][p].rnd = 0;
                    }
                }
                else if (MDataArr[i][p].rnd == 0) {
                    MDataArr[i][p].sy += down;
                    if (MDataArr[i][p].sy <= 0.1) {
                        MDataArr[i][p].rnd = 1;
                    }
                }
            }
        }
        glutTimerFunc(speedPlus, m_speedPlus, 0);
    }
}
GLvoid m_speedMinus(int value) {
    //육면체들이 위 아래로 움직이고, 멈추기 (MDataArr[i][p].sy이 3을 넘어서면 다시 내려오고, 0보다 작으면 다시 올라오기)
    if (s.isminus == 1) {
        for (int i = 0; i < h; i++) {
            for (int p = 0; p < w; p++) {
                //각 객체가 위아래로 움직이게 하기  
                if (MDataArr[i][p].rnd == 1) {
                    MDataArr[i][p].sy += up;
                    if (MDataArr[i][p].sy > 4) {
                        MDataArr[i][p].rnd = 0;
                    }
                }
                else if (MDataArr[i][p].rnd == 0) {
                    MDataArr[i][p].sy += down;
                    if (MDataArr[i][p].sy <= 0.1) { 
                        MDataArr[i][p].rnd = 1;
                    }
                }
            }
        }
        glutTimerFunc(speedMinus, m_speedMinus, 0);
    }
}

GLvoid camerax(int value) {
    if (s.isx == 1 && s.cntx % 2 != 0) {
        angle = glm::radians(Camangle);
        cX = glm::mat4(1.0f);
        cX = glm::rotate(cX, angle, glm::vec3(1, 0, 0));
        Camangle += 5;
        glutTimerFunc(200, camerax, 0);
    }
}
GLvoid cameraX(int value) {
    if (s.isX == 1 && s.cntX % 2 != 0) {
        angle = glm::radians(Camangle);
        cX = glm::mat4(1.0f);
        cX = glm::rotate(cX, angle, glm::vec3(1, 0, 0));
        Camangle -= 5;
        glutTimerFunc(200, cameraX, 0);
    }
}
GLvoid cameray(int value) {
    if (s.isy == 1 && s.cnty % 2 != 0) { 
        angle = glm::radians(Camangle);
        cY = glm::mat4(1.0f);
        cY = glm::rotate(cY, angle, glm::vec3(0, 1, 0));  
        Camangle += 5; 
        glutTimerFunc(200, cameray, 0);  
    } 
} 
GLvoid cameraY(int value) { 
    if (s.isY == 1 && s.cntY % 2 != 0) { 
        angle = glm::radians(Camangle);
        cY = glm::mat4(1.0f);
        cY = glm::rotate(cY, angle, glm::vec3(0, 1, 0));
        Camangle -= 5;
        glutTimerFunc(200, cameraY, 0);
    }
} 
  
void isValid(int i, int p) {
    //미로를 만들 때 오른쪽 0,  위 1 어디로 갈 지 정하는 변수
    int randomLoca = rand() % 2;
    //오른쪽
    if (randomLoca == 0) {
        if (p + 1 > w || MDataArr[i][p + 1].is == 0) {
            currenti = i;
            currentp = p;
        }
        else {
            MDataArr[i][p + 1].is = 0;
            currenti = i;
            currentp = p + 1;
        }
    }
    //위 
    else if (randomLoca == 1) {
        if (i + 1 > h || MDataArr[i + 1][p].is == 0) {
            currenti = i;
            currentp = p;
        }
        else {
            MDataArr[i + 1][p].is = 0;
            currenti = i + 1;
            currentp = p;
        }
    }
} 
void isValid2(int i, int p) {
    //미로를 만들 때 오른쪽 0,  아래1 어디로 갈 지 정하는 변수 
    int randomLoca = rand() % 2;
    //오른쪽
    if (randomLoca == 0) {
        if (p + 1 > w) {
            currenti = i;
            currentp = p;
        }
        else {
            MDataArr[i][p + 1].is = 0;
            currenti = i;
            currentp = p + 1;
        }
    }
    //아래
    else if (randomLoca == 1) {
        if (i - 1 < 0) {
            currenti = i;
            currentp = p;
        }
        else {
            MDataArr[i - 1][p].is = 0;
            currenti = i - 1;
            currentp = p;
        }
    }
}
void isValid3(int i, int p) {
    //미로를 만들 때 왼쪽 0,  위1 어디로 갈 지 정하는 변수 
    int randomLoca = rand() % 2;
    //왼쪽
    if (randomLoca == 0) {
        if (p - 1 < 0) {
            currenti = i;
            currentp = p;
        }
        else {
            MDataArr[i][p - 1].is = 0;
            currenti = i;
            currentp = p - 1;
        }
    }
    //위 
    else if (randomLoca == 1) {
        if (i + 1 > h) {
            currenti = i;
            currentp = p;
        }
        else {
            MDataArr[i + 1][p].is = 0;
            currenti = i + 1;
            currentp = p;
        }
    }
}
void isValid4(int i, int p) {
    //미로를 만들 때 왼쪽 0,  아래1 어디로 갈 지 정하는 변수 
    int randomLoca = rand() % 2;
    //왼쪽
    if (randomLoca == 0) {
        if (p - 1 < 0) {
            currenti = i;
            currentp = p;
        }
        else {
            MDataArr[i][p - 1].is = 0;
            currenti = i;
            currentp = p - 1;
        }
    }
    //아래
    else if (randomLoca == 1) {
        if (i - 1 < 0) {
            currenti = i;
            currentp = p;
        }
        else {
            MDataArr[i - 1][p].is = 0;
            currenti = i - 1;
            currentp = p;
        }
    }
}

GLvoid makeMiro() {
    //미로를 만들 때 현재 미로가 만들어진 지점이 어디인지    
    MDataArr[0][0].is = 0;
    int i{};
    int p{};
    //(0,0)에서 start
    while (true) {
        i = currenti;
        p = currentp;
        isValid(i, p);
        if (currenti == h - 1) {
            break;
        }
    }
    //(h/2,0)에서 start
    MDataArr[h -1][0].is = 0;
    currenti = h-1;
    currentp = 0;
    while (true) {
        i = currenti;
        p = currentp;
        isValid2(i, p);
        if (currenti ==h/2 )   
            break;
    }
    //(0,w-1)에서 start 
    MDataArr[0][w - 1].is = 0;
    currenti = 0;
    currentp = w - 1;
    while (true) {
        i = currenti;
        p = currentp;
        isValid3(i, p);
        if (currenti == h - 1)
            break;
    }
    //(h-1,w-1)에서 start  
    MDataArr[h - 1][w - 1].is = 0;
    currenti = h - 1;
    currentp = w - 1;
    while (true) {
        i = currenti;
        p = currentp;
        isValid4(i, p);
        if (currenti == h / 2)
            break;
    }
}

//충돌체크함수 
int CollidCheck() {
    //캐릭터가 현재 있는 배열의 범위를 먼저 체크하기 
    for (int i = 0; i < h; i++) {
        for (int p = 0; p < w; p++) {
            //캐릭터가 어디에 있는가 ? 
            if (((MDataArr[i][p].lx) <= characLX && (MDataArr[i][p].rx) >= characRX)
                && ((MDataArr[i][p].dz) <= characDZ && (MDataArr[i][p].uz) >= characUZ)) {
                inCharaci = i;
                inCharacp = p;
            }
        }
    }

    //characRX가 충돌하였는지 확인 
    if (inCharacp + 1 != w) {
        if (MDataArr[inCharaci][inCharacp + 1].is == 1) {
            if (characRX >= MDataArr[inCharaci][inCharacp + 1].lx) {
                return 1;
            }
        }
    }
    //characLX가 충동하였는지 확인 
    if (inCharacp - 1 != -1) {
        if (MDataArr[inCharaci][inCharacp - 1].is == 1) {
            if (characLX <= MDataArr[inCharaci][inCharacp - 1].rx) {
                return 1;
            }
        }
    }
    //characDZ가 충돌하였는지 확인 
    if (inCharaci - 1 != -1) {
        if (MDataArr[inCharaci - 1][inCharacp].is == 1) {
            if (characDZ <= MDataArr[inCharaci - 1][inCharacp].uz) {
                return 1;
            }
        }
    }
    //characUZ가 충돌하였는지 확인  
    if (inCharaci + 1 != h) {
        if (MDataArr[inCharaci + 1][inCharacp].is == 1) {
            if (characUZ >= MDataArr[inCharaci + 1][inCharacp].dz) {
                return 1;
            }
        }
    }
    return 0;
}

//Pink 캐릭터 움직임 함수 
GLvoid Up() {
    t = glm::translate(t, glm::vec3(0, 0, -0.01));
    characUZ -= 0.01;
    characDZ -= 0.01;
    if (s.is1ver == 1) {
        cameraPosOne = glm::vec3(characRX + 0.05, 0.56, characUZ);
        cameraDirectionOne = glm::vec3(characRX + 0.1, 0.56, characUZ);
    }
    if (CollidCheck()) {
        t = glm::translate(t, glm::vec3(0, 0, 0.01));
        characUZ += 0.01;
        characDZ += 0.01;
        if (s.is1ver == 1) {
            cameraPosOne = glm::vec3(characRX + 0.05, 0.56, characUZ);
            cameraDirectionOne = glm::vec3(characRX + 0.1, 0.56, characUZ);
        }
    }
}
GLvoid Left() {
    t = glm::translate(t, glm::vec3(-0.01, 0, 0));
    characLX -= 0.01;
    characRX -= 0.01;
    if (s.is1ver == 1) {
        cameraPosOne = glm::vec3(characRX + 0.05, 0.56, characUZ);
        cameraDirectionOne = glm::vec3(characRX + 0.1, 0.56, characUZ);
    }
    if (CollidCheck()) {
        t = glm::translate(t, glm::vec3(0.01, 0, 0));
        characLX += 0.01;
        characRX += 0.01;
        if (s.is1ver == 1) {
            cameraPosOne = glm::vec3(characRX + 0.05, 0.56, characUZ);
            cameraDirectionOne = glm::vec3(characRX + 0.1, 0.56, characUZ);
        }
    }
}
GLvoid Right() {
    t = glm::translate(t, glm::vec3(0.01, 0, 0));
    characLX += 0.01;
    characRX += 0.01;
    if (s.is1ver == 1) {
        cameraPosOne = glm::vec3(characRX + 0.05, 0.56, characUZ);
        cameraDirectionOne = glm::vec3(characRX + 0.1, 0.56, characUZ);
    }
    if (CollidCheck()) {
        t = glm::translate(t, glm::vec3(-0.01, 0, 0));
        characLX -= 0.01;
        characRX -= 0.01;
        if (s.is1ver == 1) {
            cameraPosOne = glm::vec3(characRX + 0.05, 0.56, characUZ);
            cameraDirectionOne = glm::vec3(characRX + 0.1, 0.56, characUZ);
        }
    }
}
GLvoid Down() {
    t = glm::translate(t, glm::vec3(0, 0, 0.01));
    characUZ += 0.01;
    characDZ += 0.01;
    if (s.is1ver == 1) {
        cameraPosOne = glm::vec3(characRX + 0.05, 0.56, characUZ);
        cameraDirectionOne = glm::vec3(characRX + 0.1, 0.56, characUZ);
    }
    if (CollidCheck()) {
        t = glm::translate(t, glm::vec3(0, 0, -0.01));
        characUZ -= 0.01;
        characDZ -= 0.01;
        if (s.is1ver == 1) {
            cameraPosOne = glm::vec3(characRX + 0.05, 0.56, characUZ);
            cameraDirectionOne = glm::vec3(characRX + 0.1, 0.56, characUZ);
        }
    }
}

GLvoid myKeyBoard(unsigned char key, int x, int y) {
    switch (key) {
        //시점 선택 
    case '1':
        s.is1ver = 1;
        s.is3ver = 0;
        cout << "-----------------" << endl;
        cout << "- 1 1인칭 시점 -" << endl;
        cout << "-----------------" << endl;
        break;
    case '3':
        s.is3ver = 1;
        s.is1ver = 0;
        cout << "-----------------" << endl;
        cout << "- 3 3인칭 시점 -" << endl;
        cout << "-----------------" << endl;
        break;
        //투영 선택 키보드 입력 
    case 'p':
        if (s.is3ver == 1) {
            s.isP = 1;
            s.isO = 0;
            cout << "-----------------" << endl;
            cout << "- p 원근투영 -" << endl;
            cout << "-----------------" << endl;
        }
        break;
    case 'o':
        if (s.is3ver == 1) {
            s.isO = 1;
            s.isP = 0;
            cout << "-----------------" << endl;
            cout << "- o 직각투영 -" << endl;
            cout << "-----------------" << endl;
        }
        break;
    case 'z':
        if (s.isP == 1) {
            cz -= 0.005;
            c2z -= 0.005;
            cameraPos = glm::vec3(0.0f, 0.7f, cz);
            cameraDirection = glm::vec3(0.0f, 0.5f, c2z);
            cout << "-----------------" << endl;
            cout << "- z z축으로 -이동  -" << endl;
            cout << "-----------------" << endl;
        }
        break;
    case'Z':
        if (s.isP == 1) {
            cz += 0.005;
            c2z += 0.005;
            cameraPos = glm::vec3(0.0f, 0.7f, cz);
            cameraDirection = glm::vec3(0.0f, 0.5f, c2z);
            cout << "-----------------" << endl;
            cout << "- Z z축으로 +이동  -" << endl;
            cout << "-----------------" << endl;
        }
        break;
        //육면체들이 위 아래로 움직이기
    case 'm':
        s.ism = 1;
        //각 객체들이 내려갈건지 올라갈건지 정하는 랜덤 변수 rnd : 0 - 내려갈것, 1 - 올라갈 것  
        for (int i = 0; i < h; i++) {
            for (int p = 0; p < w; p++) {
                MDataArr[i][p].rnd = rand() % 2;
            }
        }
        glutTimerFunc(100, m, 0);
        cout << "-----------------" << endl;
        cout << "- m 육면체들 위아래로 이동  -" << endl;
        cout << "-----------------" << endl;
        break;
        //육면체들이 위 아래로 움직이는 것 멈추기 
    case 'M':
        s.ism = 0;
        s.isv = 0;
        s.cntv = 0;
        s.isplus = 0;
        s.isminus = 0;
        cout << "-----------------" << endl;
        cout << "- M 육면체들 위아래로 이동 멈춤  -" << endl;
        cout << "-----------------" << endl;
        break;
        //육면체들의 움직임 멈추고 낮은 높이로 변함 
    case 'v':
        srand((unsigned int)time(NULL));
        s.isv = 1;
        s.cntv += 1;
        if (s.cntv % 2 != 0) {
            s.ism = 0;
            for (int i = 0; i < h; i++) {
                for (int p = 0; p < w; p++) {
                    MDataArr[i][p].sy = 0.5;
                }
            }
        }
        else {
            s.ism = 1;
            for (int i = 0; i < h; i++) {
                for (int p = 0; p < w; p++) {
                    MDataArr[i][p].rnd = rand() % 2;
                }
            }
            glutTimerFunc(100, m, 0);
        }
        cout << "-----------------" << endl;
        cout << "- v 육면체들 움직임 멈추고 낮은 높이로 변하기  -" << endl;
        cout << "-----------------" << endl;
        break;
    case '+':
        s.ism = 0;
        s.isplus = 1;
        s.isminus = 0;
        glutTimerFunc(speedPlus, m_speedPlus, 0);
        cout << "-----------------" << endl;
        cout << "- '+' 육면체들 이동 속도 증가  -" << endl;
        cout << "-----------------" << endl;
        break;
    case '-':
        s.ism = 0;
        s.isplus = 0;
        s.isminus = 1;
        glutTimerFunc(speedMinus, m_speedMinus, 0);
        cout << "-----------------" << endl;
        cout << "- '-' 육면체들 이동속도 감소  -" << endl;
        cout << "-----------------" << endl;
        break;
        //카메라 변환 
    case 'x':
        if (s.is3ver == 1) {
            s.isx = 1;
            s.isX = 0;
            s.cntx += 1;
            if (s.cntx % 2 != 0) {
                glutTimerFunc(200, camerax, 0);    //x를 다시 누르면 멈춤
            }
            cout << "-----------------" << endl;
            cout << "- x 카메라 x축으로 양의 방향 회전 -" << endl;
            cout << "-----------------" << endl;
        }
        break;
    case 'X':
        if (s.is3ver == 1) {
            s.isX = 1;
            s.isx = 0;
            s.cntX += 1;
            if (s.cntX % 2 != 0) {
                glutTimerFunc(200, cameraX, 0);  //X를 다시 누르면 멈춤 
            }
            cout << "-----------------" << endl;
            cout << "- X 카메라 x축으로 음의 방향 회전 -" << endl;
            cout << "-----------------" << endl;
        }

        break;
    case 'y':
        if (s.is3ver == 1) {
            s.isy = 1;
            s.isY = 0;
            s.cnty += 1;
            if (s.cnty % 2 != 0) {
                glutTimerFunc(200, cameray, 0);  //y를 누르면 다시 멈춤 
            }
            cout << "-----------------" << endl;
            cout << "- y 카메라 y축으로 양의 방향 회전 -" << endl;
            cout << "-----------------" << endl;
        }
        break;
    case 'Y':
        if (s.is3ver == 1) {
            s.isy = 0;
            s.isY = 1;
            s.cntY += 1;
            if (s.cntY % 2 != 0) {
                glutTimerFunc(200, cameraY, 0);  //Y를 누르면 다시 멈춤  
            }
            cout << "-----------------" << endl;
            cout << "- Y 카메라 y축으로 양의 방향 회전 -" << endl;
            cout << "-----------------" << endl;
        }
        break;
        //미로 만들기  
    case 'r':
        srand((unsigned int)time(NULL));
        if (s.isr == 0) {
            makeMiro();
            s.miniMap = 1;
        }
        s.isr = 1;
        cout << "-----------------" << endl;
        cout << "- r 미로 생성 -" << endl;
        cout << "-----------------" << endl;
        break;
        //캐릭터 객체 움직임 시작 
    case 'w': //위 
        s.current = 'w';
        Up();
        cout << "-----------------" << endl;
        cout << "- w 객체 위로 이동  -" << endl;
        cout << "-----------------" << endl;
        break;
    case 'a': //왼 
        s.current = 'a';
        Left();
        cout << "-----------------" << endl;
        cout << "- a 객체 왼쪽으로 이동  -" << endl;
        cout << "-----------------" << endl;
        break;
    case 's': //아래
        s.current = 's';
        Down();
        cout << "-----------------" << endl;
        cout << "- s 객체 아래로 이동  -" << endl;
        cout << "-----------------" << endl;
        break;
    case 'd': //오 
        s.current = 'd';
        Right();
        cout << "-----------------" << endl;
        cout << "- d 객체 오른쪽으로 이동  -" << endl;
        cout << "-----------------" << endl;
        break;
        //초기화 
    case 'c':
        turn = 0;
        s.is3ver = 1; 
        s.is1ver = 0;  

        t = glm::mat4(1.0f);
        cX = glm::mat4(1.0f);
        cY = glm::mat4(1.0f);

        cz = 0.5;
        c2z = 0.2;
        cameraPos = glm::vec3(0.0f, 0.7f, cz);
        cameraDirection = glm::vec3(0.0f, 0.5f, c2z);

        characLX = -0.49;
        characRX = -0.46;
        characUZ = -0.46;
        characDZ = -0.49;
        cameraPosOne = glm::vec3(characRX + 0.05, 0.56, characUZ);
        cameraDirectionOne = glm::vec3(characRX + 0.1, 0.56, characUZ);

        CubeModel = glm::mat4(1.0f);
        MountinModel = glm::mat4(1.0f);
        CharacterModel = glm::mat4(1.0f);

        projection = glm::mat4(1.0f);
        view = glm::mat4(1.0f);

        delete[] MDataArr;
        Init();   
        UserFunc();  
        cout << "-----------------" << endl;
        cout << "- c 모든 값 초기화  -" << endl;
        cout << "-----------------" << endl;
        break;
        //종료 
    case 'q':
        cout << "-----------------" << endl;
        cout << "- q 종료 -" << endl;
        cout << "-----------------" << endl;
        puts("- Program End - ");
        delete[] MDataArr;
        exit(-1);
    }
}

GLvoid drawScene() {
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glViewport(0, 0, 800, 800);
   
    if (s.is3ver == 1 && s.is1ver == 0) {
        View();
        Pro();
        InitLightFloor();
        Floor();

        InitLightMountin();
        for (int i = 0; i < h; i++) {
            for (int p = 0; p < w; p++) {
                if (MDataArr[i][p].is == 1) {
                    Mountins(i, p);
                }
            }
        }
        if (s.isr == 1) {

            InitLightCharac();
            if (s.current == 'w' || s.current == 'a' || s.current == 'd' || s.current == 's') {
                //캐릭터 움직이기 시작     
                TransPinkCharac();
            }
            else {
                PinkCharac();
            }
        }
    }
    else if (s.is1ver == 1 && s.is3ver == 0) {
        ViewOne();
        ProOne();
        InitLightFloor();
        Floor();
        InitLightMountin();
        for (int i = 0; i < h; i++) {
            for (int p = 0; p < w; p++) {
                if (MDataArr[i][p].is == 1) {
                    Mountins(i, p);
                }
            }
        }
        if (s.isr == 1) {
            InitLightCharac();
            if (s.current == 'w' || s.current == 'a' || s.current == 'd' || s.current == 's') {
                //캐릭터 움직이기 시작     
                TransPinkCharac();
            }
            else {
                PinkCharac();
            }
        }
    }
    //------------------------------------------------------------------------------------------
    //미니맵 뷰 
    if (s.miniMap == 1) {
        glViewport(700, 700, 300, 300);
        View2();
        Pro2();
        InitLightFloor();
        Floor();
        InitLightMountin();
        for (int i = 0; i < h; i++) {
            for (int p = 0; p < w; p++) {
                if (MDataArr[i][p].is == 1) {
                    Mountins(i, p);
                }
            }
        }
        if (s.isr == 1) {
            InitLightCharac();
            if (s.current == 'w' || s.current == 'a' || s.current == 'd' || s.current == 's') {
                //캐릭터 움직이기 시작     
                TransPinkCharac();
            }
            else {
                PinkCharac();
            }
        }
    }
    //------------------------------------------------------------------------------------------ 
    turn += 1;
    glutPostRedisplay();
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("숙제1");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }
    else
        cout << "GLEW Initialized\n";

    UserFunc();

    make_vertexShaders();
    make_fragmentShaders();

    shaderID = make_shaderProgram();
    InitBuffer();

    glutDisplayFunc(drawScene);
    glutKeyboardFunc(myKeyBoard);

    glutMainLoop();
    return 0;
}
