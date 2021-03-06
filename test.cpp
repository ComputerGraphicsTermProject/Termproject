#define STB_IMAGE_IMPLEMENTATION     
#include "TermPro.h"             
#include "stb_image.h"     
#include <mmsystem.h> 
#include <Windows.h>

#pragma comment(lib, "winmm.lib")    
#include "Mmsystem.h"
#include "Digitalv.h"
MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms; 
DWORD m_dwDeviceID; 
MCI_OPEN_PARMS mciOpen; 
MCI_PLAY_PARMS mciPlay; 

int dwID;

//Sound
GLvoid background_Sound() {
    mciOpen.lpstrElementName = L"배경음.wav"; // 파일 경로 입력 
    mciOpen.lpstrDeviceType = L"mpegvideo";

    mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
        (DWORD)(LPVOID)&mciOpen);

    dwID = mciOpen.wDeviceID;
    mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // play & repeat
        (DWORD)(LPVOID)&m_mciPlayParms);
}
GLvoid scream_Sound() { 

    mciOpen.lpstrElementName = L"비명소리2.wav"; // 파일 경로 입력  
    mciOpen.lpstrDeviceType = L"mpegvideo";

    int Sound1 = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
        (DWORD)(LPVOID)&mciOpen);

    dwID = mciOpen.wDeviceID;
    mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, // play & repeat   
        (DWORD)(LPVOID)&m_mciPlayParms);
} 
GLvoid scream_background_Sound() {
    mciOpen.lpstrElementName = L"배경음악.wav"; // 파일 경로 입력  
    mciOpen.lpstrDeviceType = L"mpegvideo";

    int Sound1 = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
        (DWORD)(LPVOID)&mciOpen);

    dwID = mciOpen.wDeviceID;
    mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, // play & repeat   
        (DWORD)(LPVOID)&m_mciPlayParms);
}
GLvoid Down_Sound() {
    mciOpen.lpstrElementName = L"슉.wav"; // 파일 경로 입력    
    mciOpen.lpstrDeviceType = L"mpegvideo";

    int Sound1 = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
        (DWORD)(LPVOID)&mciOpen);

    dwID = mciOpen.wDeviceID;
    mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, // play & repeat   
        (DWORD)(LPVOID)&m_mciPlayParms);
}
GLvoid win_Sound() { 
    mciOpen.lpstrElementName = L"탈출.wav"; // 파일 경로 입력    
    mciOpen.lpstrDeviceType = L"mpegvideo";

    int Sound1 = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
        (DWORD)(LPVOID)&mciOpen);

    dwID = mciOpen.wDeviceID;
    mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, // play & repeat   
        (DWORD)(LPVOID)&m_mciPlayParms);
}
GLvoid ending_Sound() {
    mciOpen.lpstrElementName = L"엔딩.wav"; // 파일 경로 입력     
    mciOpen.lpstrDeviceType = L"mpegvideo";

    int Sound1 = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
        (DWORD)(LPVOID)&mciOpen);

    dwID = mciOpen.wDeviceID;
    mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, // play & repeat   
        (DWORD)(LPVOID)&m_mciPlayParms);
}

int num_Triangle[2];
objReader obj[2];
GLuint VAO[3], VBO_pos[3], VBO_normal[3], VBO_uv[3];

float cx = Robot_X, cy = Robot_Y + 0.35, cz = Robot_Z + 0.000001; 
float c2x = Robot_X, c2y = Robot_Y + 0.3, c2z = Robot_Z; 
//float cx =0, cy = 10.0, cz = 0.01;
//float c2x = 0, c2y = 0, c2z = 0;
glm::vec3 cameraPos = glm::vec3(cx, cy, cz);
glm::vec3 cameraDirection = glm::vec3(c2x, c2y, c2z); 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat lightX = 0.0, lightY = 10.0, lightZ = 0.0; 
GLfloat lightR = 0.5, lightG =0.5, lightB = 0.5;  
//---------------------------------------------- 
struct BB {
    float minx;
    float minz;
    float maxx;
    float maxz;

    BB(float minX, float minZ, float maxX, float maxZ)
    {
        this->minx = minX;
        this->minz = minZ;
        this->maxx = maxX;
        this->maxz = maxZ;
    }
};
bool Collide(BB a, BB b)
{
    if (a.maxx <= b.minx)
        return false;
    if (a.minx >= b.maxx)
        return false;
    if (a.maxz <= b.minz)
        return false;
    if (a.minz >= b.maxz)
        return false;
    return true;
} 
//오 
BB getbb_wall1() {
    return BB(0.5f, -0.5f, 0.55f, 0.5f);
} 
//왼 
BB getbb_wall2() {
    return BB(-0.55f, -0.5f, -0.5f, 0.5f);
}  
//위
BB getbb_wall3() {
    return BB(-1.0f, -0.55f, 1.0f, -0.5f);  
}  
//아래 
BB getbb_wall4() {
    return BB(-1.0f, 0.49f, 1.0f, 0.55f);
} 
BB getbb_monster(float centerx, float centerz)
{
    return BB(centerx - 0.015f, centerz - 0.015, centerx + 0.015f, centerz + 0.015f);

} 
BB getbb_robot(float centerx, float centerz)
{
    return BB(centerx - 0.015f, centerz-0.005, centerx + 0.015f, centerz + 0.03f);    
}
BB getbb_cube(float centerx, float centerz)
{
    return BB(centerx - 0.025f, centerz - 0.025, centerx + 0.025f, centerz + 0.025);
}
BB getbb_teleport(float centerx, float centerz)
{
    return BB(centerx - 0.015f, centerz - 0.015f, centerx + 0.015f, centerz + 0.015f);

}
BB getbb_exit(float centerx, float centerz)
{
    return BB(centerx - 0.015f, centerz - 0.015f, centerx + 0.015f, centerz + 0.015f);
}

//미로의 각 중점 구하기 
float CubeCenterX[20][20];
float CubeCenterZ[20][20]; 
GLvoid GetCenterX() {
    for (int p = 0; p < 20; ++p) {
        CubeCenterX[0][p] = -0.475 + (0.05 * p);

    }
    for (int i = 0; i < 20; ++i) {
        for (int p = 0; p < 20; ++p) {
            CubeCenterX[i][p] = -0.475 + (0.05 * p);
        }
    }
}  
GLvoid GetCenterZ() {  
    for (int i = 0; i < 20; ++i) {
        for (int p = 0; p < 20; ++p) {  
            CubeCenterZ[i][p] = -0.475 + (0.05 * i);    
        }  
    }
}
//------------------------------------------------------------
GLuint texture[10];
int tLocation;
GLvoid InitTexture()
{
    int width[10], height[10], nrChannels[10];
    //--------------------------------------------------------------------------------------------
    glGenTextures(1, &texture[0]); //--- 텍스처 생성
    glBindTexture(GL_TEXTURE_2D, texture[0]); //--- 텍스처 바인딩 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("plyaercolor.jpg", &width[0], &height[0], &nrChannels[0], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의
    glUseProgram(shaderID);
    tLocation = glGetUniformLocation(shaderID, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴 
    glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
    stbi_image_free(data);

    glGenTextures(1, &texture[2]); //--- 텍스처 생성
    glBindTexture(GL_TEXTURE_2D, texture[2]); //--- 텍스처 바인딩 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data3 = stbi_load("GameOver.jpg", &width[2], &height[2], &nrChannels[2], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width[2], height[2], 0, GL_RGB, GL_UNSIGNED_BYTE, data3); //---텍스처 이미지 정의
    glUseProgram(shaderID);
    tLocation = glGetUniformLocation(shaderID, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴 
    glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
    stbi_image_free(data3);
    //------------------------------------------------------------
    glGenTextures(1, &texture[3]); //--- 텍스처 생성
    glBindTexture(GL_TEXTURE_2D, texture[3]); //--- 텍스처 바인딩 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data4 = stbi_load("mountin.jpeg", &width[3], &height[3], &nrChannels[3], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width[3], height[3], 0, GL_RGB, GL_UNSIGNED_BYTE, data4); //---텍스처 이미지 정의
    glUseProgram(shaderID);
    tLocation = glGetUniformLocation(shaderID, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴 
    glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
    stbi_image_free(data4);
    //---------------------------------------------------------------
    glGenTextures(1, &texture[4]); //--- 텍스처 생성
    glBindTexture(GL_TEXTURE_2D, texture[4]); //--- 텍스처 바인딩 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data5 = stbi_load("wall.jpg", &width[4], &height[4], &nrChannels[4], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width[4], height[4], 0, GL_RGB, GL_UNSIGNED_BYTE, data5); //---텍스처 이미지 정의
    glUseProgram(shaderID);
    tLocation = glGetUniformLocation(shaderID, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴 
    glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
    stbi_image_free(data5);
    //---------------------------------------------------------
    glGenTextures(1, &texture[5]); //--- 텍스처 생성
    glBindTexture(GL_TEXTURE_2D, texture[5]); //--- 텍스처 바인딩 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data6 = stbi_load("tile.jpg", &width[5], &height[5], &nrChannels[5], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width[5], height[5], 0, GL_RGB, GL_UNSIGNED_BYTE, data6); //---텍스처 이미지 정의
    glUseProgram(shaderID);
    tLocation = glGetUniformLocation(shaderID, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴 
    glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
    stbi_image_free(data6);
    //-------------------------------------------------------------
    glGenTextures(1, &texture[6]); //--- 텍스처 생성
    glBindTexture(GL_TEXTURE_2D, texture[6]); //--- 텍스처 바인딩 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* data7 = stbi_load("Monster.jpg", &width[6], &height[6], &nrChannels[6], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width[6], height[6], 0, GL_RGB, GL_UNSIGNED_BYTE, data7); //---텍스처 이미지 정의
    glUseProgram(shaderID); 
    tLocation = glGetUniformLocation(shaderID, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴 
    glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정 
    stbi_image_free(data7); 
    //------------------------------------------------------------------------ 
    glGenTextures(1, &texture[7]); //--- 텍스처 생성
    glBindTexture(GL_TEXTURE_2D, texture[7]); //--- 텍스처 바인딩 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data8 = stbi_load("win.JPG", &width[7], &height[7], &nrChannels[7], 0); 
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width[7], height[7], 0, GL_RGB, GL_UNSIGNED_BYTE, data8); //---텍스처 이미지 정의
    glUseProgram(shaderID);
    tLocation = glGetUniformLocation(shaderID, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴 
    glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정  
    stbi_image_free(data8);  
    //--------------------------------------------------------------------------
    glGenTextures(1, &texture[8]); //--- 텍스처 생성
    glBindTexture(GL_TEXTURE_2D, texture[8]); //--- 텍스처 바인딩 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* data9 = stbi_load("boy.jpg", &width[8], &height[8], &nrChannels[8], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width[8], height[8], 0, GL_RGB, GL_UNSIGNED_BYTE, data9); //---텍스처 이미지 정의
    glUseProgram(shaderID);
    tLocation = glGetUniformLocation(shaderID, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴 
    glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
    stbi_image_free(data9);
    //------------------------------------------------------------------------
    glGenTextures(1, &texture[9]); //--- 텍스처 생성
    glBindTexture(GL_TEXTURE_2D, texture[9]); //--- 텍스처 바인딩 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data10 = stbi_load("white.jpg", &width[9], &height[9], &nrChannels[9], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width[9], height[9], 0, GL_RGB, GL_UNSIGNED_BYTE, data10); //---텍스처 이미지 정의
    glUseProgram(shaderID);
    tLocation = glGetUniformLocation(shaderID, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴 
    glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
    stbi_image_free(data10); 
}
GLvoid InitBuffer() {
    num_Triangle[0] = obj[0].loadObj("Cube.obj"); 
    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO_pos);
    glGenBuffers(3, VBO_normal);
    glGenBuffers(3, VBO_uv);

    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos[0]);
    glBufferData(GL_ARRAY_BUFFER, obj[0].outvertex.size() * sizeof(glm::vec3), &obj[0].outvertex[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_normal[0]);
    glBufferData(GL_ARRAY_BUFFER, obj[0].outnormal.size() * sizeof(glm::vec3), &obj[0].outnormal[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_uv[0]);
    glBufferData(GL_ARRAY_BUFFER, obj[0].outuv.size() * sizeof(glm::vec3), &obj[0].outuv[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(2);

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
            //크기 스케일  
            MDataArr[i][p].sx = 1.0 / 20;
            MDataArr[i][p].sy = 0.3;
            MDataArr[i][p].sz = 1.0 / 20;
            //위치 이동하기  
            MDataArr[i][p].x = (-0.5 + (1.0 / w) / 2) + ((1.0 / w) * p);
            MDataArr[i][p].y = 0;
            MDataArr[i][p].z = (-0.5 + (1.0 / h) / 2) + ((1.0 / h) * i);

            //위치 계산하기  
            MDataArr[i][p].lx = (-0.5 + ((1.0 / w) * p)); //왼      
            MDataArr[i][p].rx = (-0.5 + ((1.0 / w) * p)) + (1.0 / w); //오      
            MDataArr[i][p].uz = (-0.5 + ((1.0 / h) * i)) + (1.0 / h); //양의 방향쪽 z       
            MDataArr[i][p].dz = (-0.5 + ((1.0 / h) * i)); //음의 방향쪽 z    

            if (MDataArr[i][p].lx == float(-0.05)) {
                MDataArr[i][p].rx = 0;
            }
            if (MDataArr[i][p].dz == float(-0.05)) {
                MDataArr[i][p].uz = 0;
            }
            //--------------------------------------------------------------------

            MDataArr2[i][p].is = 0;
            //크기 스케일  
            MDataArr2[i][p].sx = 1.0 / 20;
            MDataArr2[i][p].sy = 0.3;
            MDataArr2[i][p].sz = 1.0 / 20;

            //위치 이동하기  
            MDataArr2[i][p].x = (-0.5 + (1.0 / w) / 2) + ((1.0 / w) * p);
            MDataArr2[i][p].y = 0;
            MDataArr2[i][p].z = (-0.5 + (1.0 / h) / 2) + ((1.0 / h) * i);

            //------------------------------------------------------------- 


            //위치 계산하기 
            MDataArr2[i][p].lx = (-0.5 + ((1.0 / 20) * p)); //왼     
            MDataArr2[i][p].rx = (-0.5 + ((1.0 / 20) * p)) + (1.0 / 20); //오    
            MDataArr2[i][p].uz = (-0.5 + ((1.0 / 20) * i)) + (1.0 / 20); //양의 방향쪽 z      
            MDataArr2[i][p].dz = (-0.5 + ((1.0 / 20) * i)); //음의 방향쪽 z   

            if (MDataArr2[i][p].lx == float(-0.05)) {
                MDataArr2[i][p].rx = 0;
            }
            if (MDataArr2[i][p].dz == float(-0.05)) {
                MDataArr2[i][p].uz = 0;
            }

            //y = 0.62;
        }
    }
    GetCenterX();
    GetCenterZ();
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
    Angle = glm::radians(20.0f);
    projection = glm::perspective(Angle, (float)1000 / (float)1000, 1.0f, 50.0f);
    projection = glm::translate(projection, glm::vec3(0.0, 0.0, -1.0));
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
    glUniform3f(objColorLocation,0.5,0.6,0.7);

    MiroModel = glm::mat4(1.0f);
    MiroModel = glm::translate(MiroModel, glm::vec3(MDataArr[i][p].x, 0.3, MDataArr[i][p].z));
    MiroModel = glm::scale(MiroModel, glm::vec3(MDataArr[i][p].sx, MDataArr[i][p].sy, MDataArr[i][p].sz));
    MiroModel = glm::translate(MiroModel, glm::vec3(0, -0.3, 0));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MiroModel));
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle[0]);
}
GLvoid Mountins2(int i, int p) {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation,0.3,0.4,0.5); 

    MiroModel2 = glm::mat4(1.0f);
    MiroModel2 = glm::translate(MiroModel2, glm::vec3(MDataArr2[i][p].x, 0.7, MDataArr2[i][p].z)); // 이 값을 바꿔줌 stage2 바닥의 위치 (2층)
    MiroModel2 = glm::scale(MiroModel2, glm::vec3(MDataArr2[i][p].sx, MDataArr2[i][p].sy, MDataArr2[i][p].sz));
    MiroModel2 = glm::translate(MiroModel2, glm::vec3(0, -0.3, 0));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MiroModel2));
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle[0]);
}
GLvoid Floor() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation, 0.2, 0.2, 0.2);

    CubeModel = glm::mat4(1.0f);
    CubeModel = glm::scale(CubeModel, glm::vec3(1, 0.1, 1));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CubeModel));
    glBindTexture(GL_TEXTURE_2D, texture[5]);  
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle[0]); 
} 
GLvoid Floor2() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation,0.1,0.1,0.1);

    CubeModel = glm::mat4(1.0f);
    CubeModel = glm::scale(CubeModel, glm::vec3(1, 0.1, 1));
    CubeModel = glm::translate(CubeModel, glm::vec3(0.0, 4.1, 0)); // y값 4.1로 수정 (2층)

    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CubeModel));

    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}
//벽면
GLvoid Wall() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation, 1, 1, 1);
    glUniform3f(alphaLocation, 1, 1, 1);
    //오른쪽
    CubeModel = glm::mat4(1.0f); glBindTexture(GL_TEXTURE_2D, texture[4]); glDrawArrays(GL_TRIANGLES, 0, num_Triangle[0]);
    CubeModel = glm::translate(CubeModel, glm::vec3(0.53, 0.5, 0));
    CubeModel = glm::scale(CubeModel, glm::vec3(0.05, 1, 1.05));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CubeModel));
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle[0]);
    //왼쪽  
    CubeModel = glm::mat4(1.0f);
    CubeModel = glm::translate(CubeModel, glm::vec3(-0.53, 0.5, 0));
    CubeModel = glm::scale(CubeModel, glm::vec3(0.05, 1, 1.05));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CubeModel));
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle[0]);
    //아래 
    CubeModel = glm::mat4(1.0f);
    CubeModel = glm::translate(CubeModel, glm::vec3(0, 0.5, 0.53));
    CubeModel = glm::scale(CubeModel, glm::vec3(1, 1, 0.05));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CubeModel));
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle[0]);

    //위
    CubeModel = glm::mat4(1.0f);
    CubeModel = glm::translate(CubeModel, glm::vec3(0, 0.5, -0.53));
    CubeModel = glm::scale(CubeModel, glm::vec3(1, 1, 0.05));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(CubeModel));
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle[0]);
} 
GLvoid Robot() { 
    glUseProgram(shaderID); 
    glBindVertexArray(VAO[0]); 
    InitLight();  
    //몸 
    RobotModel = glm::mat4(1.0f); 
    RobotModel = RobotModel * TJ_Robot * TT_Robot * TR_Robot; 
    RobotModel = glm::translate(RobotModel, glm::vec3(R_x - 0.025, R_y + 0.02, R_z)); 
    RobotModel = glm::scale(RobotModel, glm::vec3(0.03, 0.03, 0.03)); 
    modelLocation = glGetUniformLocation(shaderID, "modelTransform"); 
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RobotModel)); 
    glUniform3f(objColorLocation, 1, 1, 1); 
    glUniform3f(alphaLocation, 1, 1, 1);  
    glBindTexture(GL_TEXTURE_2D, texture[0]); 
    glDrawArrays(GL_TRIANGLES, 0, 36); 
     
    //얼굴   
    Angle = glm::radians(-40.0);  
    RobotModel = glm::mat4(1.0f);
    RobotModel = RobotModel * TJ_Robot * TT_Robot * TR_Robot;
    RobotModel = glm::translate(RobotModel, glm::vec3(R_x - 0.025, R_y + 0.04, R_z));
    RobotModel = glm::rotate(RobotModel, Angle,glm::vec3(1,0,0)); 
    RobotModel = glm::scale(RobotModel, glm::vec3(0.025, 0.025, 0.025));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RobotModel));
    glUniform3f(objColorLocation, 1, 1, 1);

    glUniform3f(objColorLocation, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glDrawArrays(GL_TRIANGLES, 6, 6);

    glUniform3f(objColorLocation, 0.545098,0.270588,0.0745); 
    glBindTexture(GL_TEXTURE_2D, texture[9]);   
    glDrawArrays(GL_TRIANGLES, 0, 6); 
    glDrawArrays(GL_TRIANGLES, 12, 24);

    //왼팔 
    Angle = glm::radians(-10.0);
    RobotModel = glm::mat4(1.0f);
    RobotModel = RobotModel * TJ_Robot * TT_Robot * TR_Robot * TlArm;
    RobotModel = glm::translate(RobotModel, glm::vec3(R_x - 0.045, R_y + 0.02, R_z + 0.01));
    RobotModel = glm::rotate(RobotModel, Angle, glm::vec3(0, 0, 1));
    RobotModel = glm::scale(RobotModel, glm::vec3(0.01, 0.03, 0.005));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RobotModel));
    glUniform3f(objColorLocation, 1, 1, 1);
    glUniform3f(alphaLocation, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //오른팔 
    Angle = glm::radians(10.0);
    RobotModel = glm::mat4(1.0f);
    RobotModel = RobotModel * TJ_Robot * TT_Robot * TR_Robot * TrArm;
    RobotModel = glm::translate(RobotModel, glm::vec3(R_x-0.005, R_y + 0.02, R_z + 0.01));
    RobotModel = glm::rotate(RobotModel, Angle, glm::vec3(0, 0, 1));
    RobotModel = glm::scale(RobotModel, glm::vec3(0.01, 0.03, 0.005));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RobotModel));
    glUniform3f(objColorLocation, 1, 1, 1);
    glUniform3f(alphaLocation, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    //왼다리  
    RobotModel = glm::mat4(1.0f);
    RobotModel = RobotModel * TJ_Robot * TT_Robot * TR_Robot * TlLeg;
    RobotModel = glm::translate(RobotModel, glm::vec3(R_x - 0.038, R_y - 0.01, R_z - 0.005));
    RobotModel = glm::scale(RobotModel, glm::vec3(0.01, 0.032, 0.01));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RobotModel));
    glUniform3f(objColorLocation, 1, 1, 1);
    glUniform3f(alphaLocation, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //오른다리  
    RobotModel = glm::mat4(1.0f);
    RobotModel = RobotModel * TJ_Robot * TT_Robot * TR_Robot * TrLeg;
    RobotModel = glm::translate(RobotModel, glm::vec3(R_x - 0.012, R_y - 0.01, R_z - 0.005));
    RobotModel = glm::scale(RobotModel, glm::vec3(0.01, 0.032, 0.01));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RobotModel));
    glUniform3f(objColorLocation, 1, 1, 1);
    glUniform3f(alphaLocation, 1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, texture[0]); 
    glDrawArrays(GL_TRIANGLES, 0, 36);
}  
//---------------------------------
class Monster_info {
public:
    float MX{};
    float MY{};
    float MZ{};

    float Monster_X{};
    float Monster_Y{};
    float Monster_Z{};

    int floor{};
    int automoving = 0;
    int current = 0;
    glm::mat4 TT_Monster = glm::mat4(1.0f);
    glm::mat4 TR_Monster = glm::mat4(1.0f);
    glm::mat4 TJ_Monster = glm::mat4(1.0f);
    glm::mat4 MonsterModel = glm::mat4(1.0f);
    GLvoid MonsterMoving() {
        if (floor == Floor_state) {
            switch (automoving) {
            case 0:
                if (current == 2) {
                    //회전시킬 것 
                    Angle = glm::radians(90.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 3) {
                    //회전시킬 것 
                    Angle = glm::radians(270.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 1) {
                    //회전시킬 것 
                    Angle = glm::radians(180.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));

                }
                TT_Monster = glm::translate(TT_Monster, glm::vec3(0, 0, 0.003));
                automoving = 0;

                Monster_Z += 0.003;
                if (floor == 2 && Floor_state == 2) {
                    for (int i = 0; i < 20; ++i) {
                        for (int p = 0; p < 20; ++p) {
                            if (Stage2[i][p] == 1) {
                                //몬스터와 미로가 충돌 
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                                    || Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall1()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall2()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall3()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall4()) == true) {

                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0, 0, -0.003));
                                    Monster_Z -= 0.003;
                                    while (true) {
                                        automoving = rand() % 4;
                                        if (automoving != 0)
                                            break;
                                    }
                                }
                                //몬스터와 로봇이 충돌 
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_robot(Robot_X, Robot_Z)) == true)
                                {
                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0, 0, -0.003));
                                    Monster_Z -= 0.003;
                                    Bad_ending = true;
                                    scream_Sound();
                                    scream_background_Sound();
                                }
                            }
                        }
                    }
                }
                if (floor == 1 && Floor_state == 1) {
                    for (int i = 0; i < 20; ++i) {
                        for (int p = 0; p < 20; ++p) {
                            if (Stage1[i][p] == 1) {
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                                    || Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall1()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall2()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall3()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall4()) == true) {

                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0, 0, -0.003));
                                    Monster_Z -= 0.003;

                                    while (true) {
                                        automoving = rand() % 4;
                                        if (automoving != 0)
                                            break;
                                    }
                                }
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_robot(Robot_X, Robot_Z)) == true)
                                {
                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0, 0, -0.003));
                                    Monster_Z -= 0.003;
                                    Bad_ending = true;
                                    scream_Sound();
                                    scream_background_Sound();
                                }
                            }
                        }
                    }
                }
                current = 0;
                break;
            case 1:
                if (current == 0) {
                    //회전시킬 것  
                    Angle = glm::radians(180.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 0) {
                    //회전시킬 것
                    Angle = glm::radians(180.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 2) {
                    //회전시킬 것 
                    Angle = glm::radians(270.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 3) {
                    //회전시킬 것
                    Angle = glm::radians(90.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                TT_Monster = glm::translate(TT_Monster, glm::vec3(0, 0, -0.003));
                automoving = 1;
                Monster_Z -= 0.003;

                if (floor == 2 && Floor_state == 2) {
                    for (int i = 0; i < 20; ++i) {
                        for (int p = 0; p < 20; ++p) {
                            if (Stage2[i][p] == 1) {
                                //몬스터와 미로가 충돌 
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                                    || Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall1()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall2()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall3()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall4()) == true) {

                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0, 0, 0.003));
                                    Monster_Z += 0.003;

                                    while (true) {
                                        automoving = rand() % 4;
                                        if (automoving != 1)
                                            break;
                                    }
                                }
                                //로봇과 몬스터가 충돌 한 거  
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_robot(Robot_X, Robot_Z)) == true)
                                {
                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0, 0, 0.003));
                                    Monster_Z += 0.003;
                                    Bad_ending = true;
                                    scream_Sound();
                                    scream_background_Sound();
                                }

                            }
                        }
                    }
                }
                if (floor == 1 && Floor_state == 1) {
                    for (int i = 0; i < 20; ++i) {
                        for (int p = 0; p < 20; ++p) {
                            if (Stage1[i][p] == 1) {
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                                    || Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall1()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall2()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall3()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall4()) == true) {

                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0, 0, 0.003));
                                    Monster_Z += 0.003;

                                    while (true) {
                                        automoving = rand() % 4;
                                        if (automoving != 1)
                                            break;
                                    }
                                }
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_robot(Robot_X, Robot_Z)) == true)
                                {
                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0, 0, 0.003));
                                    Monster_Z += 0.003;
                                    Bad_ending = true;
                                    scream_Sound();
                                    scream_background_Sound();
                                }
                            }
                        }
                    }
                }
                current = 1;
                break;
            case 2:
                if (current == 0) {
                    //회전시킬 것  
                    Angle = glm::radians(270.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 0) {
                    //회전시킬 것
                    Angle = glm::radians(270.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 3) {
                    //회전시킬 것
                    Angle = glm::radians(180.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 1) {
                    //회전시킬 것 
                    Angle = glm::radians(90.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                TT_Monster = glm::translate(TT_Monster, glm::vec3(-0.003, 0, 0));
                automoving = 2;

                Monster_X -= 0.003;

                if (floor == 2 && Floor_state == 2) {
                    for (int i = 0; i < 20; ++i) {
                        for (int p = 0; p < 20; ++p) {
                            if (Stage2[i][p] == 1) {
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                                    || Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall1()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall2()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall3()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall4()) == true) {

                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0.003, 0, 0));
                                    Monster_X += 0.003;

                                    while (true) {
                                        automoving = rand() % 4;
                                        if (automoving != 2)
                                            break;
                                    }
                                }
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_robot(Robot_X, Robot_Z)) == true)
                                {
                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0.003, 0, 0));
                                    Monster_X += 0.003;
                                    Bad_ending = true;
                                    scream_Sound();
                                    scream_background_Sound();

                                }
                            }
                        }
                    }
                }
                if (floor == 1 && Floor_state == 1) {
                    for (int i = 0; i < 20; ++i) {
                        for (int p = 0; p < 20; ++p) {
                            if (Stage1[i][p] == 1) {
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                                    || Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall1()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall2()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall3()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall4()) == true) {

                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0.003, 0, 0));
                                    Monster_X += 0.003;


                                    while (true) {
                                        automoving = rand() % 4;
                                        if (automoving != 2)
                                            break;
                                    }
                                }
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_robot(Robot_X, Robot_Z)) == true)
                                {
                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(0.003, 0, 0));
                                    Monster_X += 0.003;
                                    Bad_ending = true;
                                    scream_Sound();
                                    scream_background_Sound();
                                }
                            }
                        }
                    }
                }
                current = 2;
                break;
            case 3:
                //로봇 오른쪽으로 걷기 
                if (current == 0) {
                    //회전시킬 것  
                    Angle = glm::radians(90.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 0) {
                    //회전시킬 것
                    Angle = glm::radians(90.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 2) {
                    //회전시킬 것 
                    Angle = glm::radians(180.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                else if (current == 1) {
                    //회전시킬 것 
                    Angle = glm::radians(270.0);
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(MX, MY, MZ));
                    TR_Monster = glm::rotate(TR_Monster, Angle, glm::vec3(0, 1, 0));
                    TR_Monster = glm::translate(TR_Monster, glm::vec3(-MX, -MY, -MZ));
                }
                TT_Monster = glm::translate(TT_Monster, glm::vec3(0.003, 0, 0));
                automoving = 3;

                Monster_X += 0.003;

                if (floor == 2 && Floor_state == 2) {
                    for (int i = 0; i < 20; ++i) {
                        for (int p = 0; p < 20; ++p) {
                            if (Stage2[i][p] == 1) {
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                                    || Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall1()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall2()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall3()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall4()) == true) {

                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(-0.003, 0, 0));
                                    Monster_X -= 0.003;


                                    while (true) {
                                        automoving = rand() % 4;
                                        if (automoving != 3)
                                            break;
                                    }
                                }
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_robot(Robot_X, Robot_Z)) == true)
                                {
                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(-0.003, 0, 0));
                                    Monster_X -= 0.003;
                                    Bad_ending = true;
                                    scream_Sound();
                                    scream_background_Sound();
                                }
                            }
                        }
                    }
                }
                if (floor == 1 && Floor_state == 1) {
                    for (int i = 0; i < 20; ++i) {
                        for (int p = 0; p < 20; ++p) {
                            if (Stage1[i][p] == 1) {
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                                    || Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall1()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall2()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall3()) == true ||
                                    Collide(getbb_monster(Monster_X, Monster_Z), getbb_wall4()) == true) {

                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(-0.003, 0, 0));
                                    Monster_X -= 0.003;

                                    while (true) {
                                        automoving = rand() % 4;
                                        if (automoving != 3)
                                            break;
                                    }
                                }
                                if (Collide(getbb_monster(Monster_X, Monster_Z), getbb_robot(Robot_X, Robot_Z)) == true)
                                {
                                    TT_Monster = glm::translate(TT_Monster, glm::vec3(-0.003, 0, 0));
                                    Monster_X -= 0.003;
                                    Bad_ending = true;
                                    scream_Sound();
                                    scream_background_Sound();
                                }
                            }
                        }
                    }
                }
                current = 3;
                break;
            }
        }
      
    }
    GLvoid DrawMonster() {
        glUseProgram(shaderID);
        glBindVertexArray(VAO[0]);
        InitLight(); 
        glUniform3f(alphaLocation, 1, 1, 1);
        //Body  
        MonsterModel = glm::mat4(1.0f);
        MonsterModel = MonsterModel * TJ_Monster * TT_Monster * TR_Monster;
        MonsterModel = glm::translate(MonsterModel, glm::vec3(MX, MY, MZ));
        MonsterModel = glm::scale(MonsterModel, glm::vec3(0.03, 0.03, 0.03));
        modelLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MonsterModel));
        glUniform3f(objColorLocation, 1, 1, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[9]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //face
        Angle = glm::radians(-40.0);
        MonsterModel = glm::mat4(1.0f);
        MonsterModel = MonsterModel * TJ_Monster * TT_Monster * TR_Monster;
        MonsterModel = glm::translate(MonsterModel, glm::vec3(MX, MY + 0.02, MZ));
        MonsterModel = glm::rotate(MonsterModel,Angle, glm::vec3(1,0,0));
        MonsterModel = glm::scale(MonsterModel, glm::vec3(0.025, 0.025, 0.025));
        modelLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MonsterModel));
        glUniform3f(objColorLocation, 1, 1, 1);  
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[6]); 
        glDrawArrays(GL_TRIANGLES, 6, 6); 

        glUniform3f(objColorLocation, 0,0,0); 
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLES, 12, 24);  
        //nose
      /*  MonsterModel = glm::mat4(1.0f);
        MonsterModel = MonsterModel * TJ_Monster * TT_Monster * TR_Monster;
        MonsterModel = glm::translate(MonsterModel, glm::vec3(MX, MY + 0.003, MZ + 0.02));
        MonsterModel = glm::scale(MonsterModel, glm::vec3(0.01, 0.01, 0.01));
        modelLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MonsterModel));
        glUniform3f(objColorLocation, 1, 1, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[9]);
        glDrawArrays(GL_TRIANGLES, 0, 36);*/

        //left Arm
        MonsterModel = glm::mat4(1.0f);
        MonsterModel = MonsterModel * TJ_Monster * TT_Monster * TR_Monster;
        Angle = glm::radians(-90.0);
        MonsterModel = glm::translate(MonsterModel, glm::vec3(MX - 0.02, MY, MZ + 0.01));
        MonsterModel = glm::rotate(MonsterModel, Angle, glm::vec3(0, 0, 1));
        MonsterModel = glm::scale(MonsterModel, glm::vec3(0.01, 0.03, 0.005));
        modelLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MonsterModel));
        glUniform3f(objColorLocation, 1, 1, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[9]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //right Arm
        MonsterModel = glm::mat4(1.0f);
        MonsterModel = MonsterModel * TJ_Monster * TT_Monster * TR_Monster;
        Angle = glm::radians(-90.0);
        MonsterModel = glm::translate(MonsterModel, glm::vec3(MX + 0.02, MY, MZ + 0.01));
        MonsterModel = glm::rotate(MonsterModel, Angle, glm::vec3(0, 0, 1));
        MonsterModel = glm::scale(MonsterModel, glm::vec3(0.01, 0.03, 0.005));
        modelLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MonsterModel));
        glUniform3f(objColorLocation, 1, 1, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[9]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //left Leg
        MonsterModel = glm::mat4(1.0f);
        MonsterModel = MonsterModel * TJ_Monster * TT_Monster * TR_Monster;
        MonsterModel = glm::translate(MonsterModel, glm::vec3(MX - 0.013, MY - 0.03, MZ - 0.005));
        MonsterModel = glm::scale(MonsterModel, glm::vec3(0.01, 0.032, 0.01));
        modelLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MonsterModel));
        glUniform3f(objColorLocation, 1, 1, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[9]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //right Leg  
        MonsterModel = glm::mat4(1.0f);
        MonsterModel = MonsterModel * TJ_Monster * TT_Monster * TR_Monster;
        MonsterModel = glm::translate(MonsterModel, glm::vec3(MX + 0.013, MY - 0.03, MZ - 0.005));
        MonsterModel = glm::scale(MonsterModel, glm::vec3(0.01, 0.032, 0.01));
        modelLocation = glGetUniformLocation(shaderID, "modelTransform");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(MonsterModel));
        glUniform3f(objColorLocation, 1, 1, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[9]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
};
Monster_info MonsterArr[16];
GLvoid InitMonster() {
    for (int i = 0; i < 8; ++i) {  
        MonsterArr[i].floor = 2; 
    }
    for (int i = 8; i < 16; ++i) {
        MonsterArr[i].floor = 1;
    }
    //2층
    MonsterArr[0].Monster_X = 0.475, MonsterArr[0].MX = 0.475;
    MonsterArr[0].Monster_Y = 0.52, MonsterArr[0].MY = 0.52;
    MonsterArr[0].Monster_Z = -0.475, MonsterArr[0].MZ = -0.475;

    MonsterArr[1].Monster_X = 0.475, MonsterArr[1].MX = 0.475;
    MonsterArr[1].Monster_Y = 0.52, MonsterArr[1].MY = 0.52;
    MonsterArr[1].Monster_Z = 0.425, MonsterArr[1].MZ = 0.425;

    MonsterArr[2].Monster_X = -0.125, MonsterArr[2].MX = -0.125;
    MonsterArr[2].Monster_Y = 0.52, MonsterArr[2].MY = 0.52;
    MonsterArr[2].Monster_Z = -0.125, MonsterArr[2].MZ = -0.125;

    MonsterArr[3].Monster_X = -0.475, MonsterArr[3].MX = -0.475;
    MonsterArr[3].Monster_Y = 0.52, MonsterArr[3].MY = 0.52;
    MonsterArr[3].Monster_Z = 0.275, MonsterArr[3].MZ = 0.275;


    MonsterArr[4].Monster_X = -0.475, MonsterArr[4].MX = -0.475;
    MonsterArr[4].Monster_Y = 0.52, MonsterArr[4].MY = 0.52;
    MonsterArr[4].Monster_Z = -0.475, MonsterArr[4].MZ = -0.475;

    MonsterArr[5].Monster_X = 0.025, MonsterArr[5].MX = 0.025;
    MonsterArr[5].Monster_Y = 0.52, MonsterArr[5].MY = 0.52;
    MonsterArr[5].Monster_Z = -0.475, MonsterArr[5].MZ = -0.475;

    MonsterArr[6].Monster_X = -0.325, MonsterArr[6].MX = -0.325;
    MonsterArr[6].Monster_Y = 0.52, MonsterArr[6].MY = 0.52;
    MonsterArr[6].Monster_Z = 0.175, MonsterArr[6].MZ = 0.175;

    MonsterArr[7].Monster_X = 0.275, MonsterArr[7].MX = 0.275;
    MonsterArr[7].Monster_Y = 0.52, MonsterArr[7].MY = 0.52;
    MonsterArr[7].Monster_Z = 0.225, MonsterArr[7].MZ = 0.225;

    //1층
    MonsterArr[8].Monster_X = 0.375, MonsterArr[8].MX = 0.375; 
    MonsterArr[8].Monster_Y = 0.2, MonsterArr[8].MY = 0.2; 
    MonsterArr[8].Monster_Z = 0.125, MonsterArr[8].MZ = 0.125; 
  
    MonsterArr[9].Monster_X = -0.475, MonsterArr[9].MX = -0.475;
    MonsterArr[9].Monster_Y = 0.2, MonsterArr[9].MY = 0.2;
    MonsterArr[9].Monster_Z = -0.325, MonsterArr[9].MZ = -0.325;

    MonsterArr[10].Monster_X = -0.475, MonsterArr[10].MX = -0.475;
    MonsterArr[10].Monster_Y = 0.2, MonsterArr[10].MY = 0.2;
    MonsterArr[10].Monster_Z = 0.075, MonsterArr[10].MZ = 0.075;

    MonsterArr[11].Monster_X = 0.125, MonsterArr[11].MX = 0.125;
    MonsterArr[11].Monster_Y = 0.2, MonsterArr[11].MY = 0.2;
    MonsterArr[11].Monster_Z = 0.375, MonsterArr[11].MZ = 0.375;

    MonsterArr[12].Monster_X = -0.075, MonsterArr[12].MX = -0.075;
    MonsterArr[12].Monster_Y = 0.2, MonsterArr[12].MY = 0.2;
    MonsterArr[12].Monster_Z = -0.325, MonsterArr[12].MZ = -0.325;

    MonsterArr[13].Monster_X = -0.175, MonsterArr[13].MX = -0.175;
    MonsterArr[13].Monster_Y = 0.2, MonsterArr[13].MY = 0.2;
    MonsterArr[13].Monster_Z = 0.425, MonsterArr[13].MZ = 0.425;

    MonsterArr[14].Monster_X = 0.125, MonsterArr[14].MX = 0.125;
    MonsterArr[14].Monster_Y = 0.2, MonsterArr[14].MY = 0.2;
    MonsterArr[14].Monster_Z = 0.175, MonsterArr[14].MZ = 0.175; 

    MonsterArr[15].Monster_X = 0.275, MonsterArr[15].MX = 0.275;
    MonsterArr[15].Monster_Y = 0.2, MonsterArr[15].MY = 0.2;
    MonsterArr[15].Monster_Z = -0.375, MonsterArr[15].MZ = -0.375;

}
GLvoid Teleport() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation, 1, 1, 1);
    glUniform3f(alphaLocation, 1, 1, 1);

    TeleportModel = glm::mat4(1.0f);
    TeleportModel = glm::translate(TeleportModel, glm::vec3(-0.33, 0.475, -0.43)); 
    TeleportModel = glm::scale(TeleportModel, glm::vec3(0.03, 0.03, 0.03));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TeleportModel));
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle[0]);
}
GLvoid ExitBox() {
    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation, 1, 1, 1);
    glUniform3f(alphaLocation, 1, 1, 1);

    ExitboxModel = glm::mat4(1.0f);
    ExitboxModel = glm::translate(ExitboxModel, glm::vec3(box_x, 0.1, box_z));
    ExitboxModel = glm::scale(ExitboxModel, glm::vec3(0.03, 0.03, 0.03));
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ExitboxModel));
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glDrawArrays(GL_TRIANGLES, 0, num_Triangle[0]);
}
GLvoid EndingBox() {

    cx = 0, cy = 1.0, cz = 0.01;
    c2x = 0, c2y = 0, c2z = 0;
    cameraPos = glm::vec3(cx, cy, cz);
    cameraDirection = glm::vec3(c2x, c2y, c2z);

    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight(); 
    glUniform3f(objColorLocation,1,1,1); 

    EndingModel = glm::mat4(1.0f);   
    EndingModel = glm::translate(EndingModel, glm::vec3(-0.12,0.1,0));        
    EndingModel = glm::scale(EndingModel, glm::vec3(0.9, 0.1, 0.9));  
    modelLocation = glGetUniformLocation(shaderID, "modelTransform");  
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(EndingModel));
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glDrawArrays(GL_TRIANGLES, 24, 6);

}
GLvoid GameoverBox() {

    cx = 0, cy = 1.0, cz = 0.01;
    c2x = 0, c2y = 0, c2z = 0;
    cameraPos = glm::vec3(cx, cy, cz);
    cameraDirection = glm::vec3(c2x, c2y, c2z); 

    glUseProgram(shaderID);
    glBindVertexArray(VAO[0]);
    InitLight();
    glUniform3f(objColorLocation, 1.0, 1.0, 1.0);
    glUniform3f(alphaLocation, Clearness, Clearness, Clearness);

    BadEndingModel = glm::mat4(1.0f); 
    BadEndingModel = glm::translate(BadEndingModel, glm::vec3(0,0.3,0));
    BadEndingModel = glm::scale(BadEndingModel, glm::vec3(1, 0.1, 1.0));  

    modelLocation = glGetUniformLocation(shaderID, "modelTransform");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(BadEndingModel));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    //제일 윗면   
    glDrawArrays(GL_TRIANGLES,24,6);   
}
GLvoid Move(int value) {
    for (int i = 0; i < 16; ++i) {  
        MonsterArr[i].MonsterMoving();
    }
    glutTimerFunc(30, Move, 0);
}
GLvoid drawScene()
{
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    Pro();
    View(); 
  
    if (Bad_ending == false && Happy_ending == false) {
        Wall();
        Robot();
        if (Floor_state == 2) {
            Floor2();
            for (int i = 0; i < 20; i++) {
                for (int p = 0; p < 20; p++) {
                    if (Stage2[i][p] == 1) {
                        Mountins2(i, p);
                    }
                }
            }
            Teleport();
            for (int i = 0; i < 8; ++i) {
                MonsterArr[i].DrawMonster();
            }
        }
        if (Floor_state == 1) {
            Floor();
            for (int i = 0; i < 20; i++) {
                for (int p = 0; p < 20; p++) {
                    if (Stage1[i][p] == 1) {
                        Mountins(i, p);
                    }
                }
            }
            ExitBox();
            for (int i = 8; i < 16; ++i) {
                MonsterArr[i].DrawMonster();
            }
        }
    }
    if (Bad_ending) {
        GameoverBox();
        over_check = true;
    }


    if (Happy_ending) {
        EndingBox();
        happy_check = true;
    }
   
  
    glutPostRedisplay();
    glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
} 
//로봇 팔 다리 움직이는 코드 
GLvoid LeftArm(int value) {
    if (leftArmtype == 0) {

        TlArm = glm::mat4(1.0f);
        Angle = glm::radians(moving1);
        TlArm = glm::rotate(TlArm, Angle, glm::vec3(1, 0, 0));
        moving1 += 0.05;
        if (moving1 > 0.8)
            leftArmtype = 1;
        glutTimerFunc(50, LeftArm, 0);
    }
    else if (leftArmtype == 1) {

        TlArm = glm::mat4(1.0f);
        Angle = glm::radians(moving1);
        TlArm = glm::rotate(TlArm, Angle, glm::vec3(1, 0, 0));
        moving1 -= 0.05;
        if (moving1 < -0.8)
            leftArmtype = 0;
        glutTimerFunc(50, LeftArm, 0);
    }

} 
GLvoid RightArm(int value) {
    if (rightArmtype == 0) {

        TrArm = glm::mat4(1.0f);
        Angle = glm::radians(moving2);
        TrArm = glm::rotate(TrArm, Angle, glm::vec3(1, 0, 0));
        moving2 -= 0.05;
        if (moving2 < -0.8)
            rightArmtype = 1;
        glutTimerFunc(50, RightArm, 0);
    }
    else if (rightArmtype == 1) {

        TrArm = glm::mat4(1.0f);
        Angle = glm::radians(moving2);
        TrArm = glm::rotate(TrArm, Angle, glm::vec3(1, 0, 0));
        moving2 += 0.05;
        if (moving2 > 0.8)
            rightArmtype = 0;
        glutTimerFunc(50, RightArm, 0);
    }
}
GLvoid LeftLeg(int value) {
    if (leftlegtype == 0) {

        TlLeg = glm::mat4(1.0f);
        Angle = glm::radians(moving3);
        TlLeg = glm::rotate(TlLeg, Angle, glm::vec3(1, 0, 0));
        moving3 -= 0.05;
        if (moving3 < -0.8)
            leftlegtype = 1;
        glutTimerFunc(50, LeftLeg, 0);
    }
    else if (leftlegtype == 1) {

        TlLeg = glm::mat4(1.0f);
        Angle = glm::radians(moving3);
        TlLeg = glm::rotate(TlLeg, Angle, glm::vec3(1, 0, 0));
        moving3 += 0.05;
        if (moving3 > 0.8)
            leftlegtype = 0;
        glutTimerFunc(50, LeftLeg, 0);
    }
}
GLvoid RightLeg(int value) {
    if (rightlegtype == 0) {

        TrLeg = glm::mat4(1.0f);
        Angle = glm::radians(moving4);
        TrLeg = glm::rotate(TrLeg, Angle, glm::vec3(1, 0, 0));
        moving4 += 0.05;
        if (moving4 > 0.8)
            rightlegtype = 1;
        glutTimerFunc(50, RightLeg, 0);
    }
    else if (rightlegtype == 1) {

        TrLeg = glm::mat4(1.0f);
        Angle = glm::radians(moving4);
        TrLeg = glm::rotate(TrLeg, Angle, glm::vec3(1, 0, 0));
        moving4 -= 0.05;
        if (moving4 < -0.8)
            rightlegtype = 0;
        glutTimerFunc(50, RightLeg, 0);
    }
}
//-----------------------------------------

GLvoid myKeyBoard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    case 's':
        //로봇 앞으로 걷기 
        if (current == 'a') {
            //회전시킬 것 
            Angle = glm::radians(90.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        else if (current == 'd') {
            //회전시킬 것 
            Angle = glm::radians(270.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        else if (current == 'w') {
            //회전시킬 것 
            Angle = glm::radians(180.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        TT_Robot = glm::translate(TT_Robot, glm::vec3(0, 0, 0.005));
        current = 's';

        Robot_Z += 0.005;
        RobotDZ += 0.005;
        RobotUZ += 0.005;

        if (Floor_state == 2) {
            for (int i = 0; i < 20; ++i) {
                for (int p = 0; p < 20; ++p) {
                    if (Stage2[i][p] == 1) {
                        //로봇과 미로가 충돌 
                        if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                            || Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall1()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall2()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall3()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall4()) == true)
                        {
                            TT_Robot = glm::translate(TT_Robot, glm::vec3(0, 0, -0.005));
                            Robot_Z -= 0.005;
                            RobotDZ -= 0.005;
                            RobotUZ -= 0.005;
                        }
                    }
                }
            }
            if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(tel_x, tel_z)) == true) {
                R_y = 0.1f;
                Floor_state = 1; 
                tel_check = true;
                Robot_Y = 0.1; 
                Down_Sound();  
            }
        }
        else if (Floor_state == 1) {
            for (int i = 0; i < 20; ++i) {
                for (int p = 0; p < 20; ++p) {
                    if (Stage1[i][p] == 1) {
                        if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                            || Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall1()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall2()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall3()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall4()) == true)
                        {
                            TT_Robot = glm::translate(TT_Robot, glm::vec3(0, 0, -0.005));
                            Robot_Z -= 0.005;
                            RobotDZ -= 0.005;
                            RobotUZ -= 0.005;
                        }
                    }
                }
            }
            if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(box_x, box_z)) == true) {
                Happy_ending = true;
                win_Sound();
                ending_Sound();
            }
        }

        //카메라 
        cx = Robot_X, cy = Robot_Y + 0.35, cz = Robot_Z + 0.000001;
        c2x = Robot_X, c2y = Robot_Y + 0.3, c2z = Robot_Z;
        cameraPos = glm::vec3(cx, cy, cz);
        cameraDirection = glm::vec3(c2x, c2y, c2z);

        break;
    case 'w':
        //로봇 뒤로 걷기 
        if (current == '0') {
            //회전시킬 것  
            Angle = glm::radians(180.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        else if (current == 's') {
            //회전시킬 것
            Angle = glm::radians(180.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        else if (current == 'a') {
            //회전시킬 것 
            Angle = glm::radians(270.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        else if (current == 'd') {
            //회전시킬 것
            Angle = glm::radians(90.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        TT_Robot = glm::translate(TT_Robot, glm::vec3(0, 0, -0.005));
        current = 'w';

        Robot_Z -= 0.005;
        RobotDZ -= 0.005;
        RobotUZ -= 0.005;

        if (Floor_state == 2) {
            for (int i = 0; i < 20; ++i) {
                for (int p = 0; p < 20; ++p) {
                    if (Stage2[i][p] == 1) {
                        //로봇과 미로가 충돌 
                        if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                            || Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall1()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall2()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall3()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall4()) == true)
                        {
                            TT_Robot = glm::translate(TT_Robot, glm::vec3(0, 0, 0.005));
                            Robot_Z += 0.005;
                            RobotDZ += 0.005;
                            RobotUZ += 0.005;
                        }
                    }
                }
            }
            if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(tel_x, tel_z)) == true) {
                R_y = 0.1f;
                Floor_state = 1;
                tel_check = true;
                Robot_Y = 0.1;
                Down_Sound();
            }
        }
        else if (Floor_state == 1) {
            for (int i = 0; i < 20; ++i) {
                for (int p = 0; p < 20; ++p) {
                    if (Stage1[i][p] == 1) {
                        if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                            || Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall1()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall2()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall3()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall4()) == true)
                        {
                            TT_Robot = glm::translate(TT_Robot, glm::vec3(0, 0, 0.005));
                            Robot_Z += 0.005;
                            RobotDZ += 0.005;
                            RobotUZ += 0.005;
                        }
                    }
                }
            }
            if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(box_x, box_z)) == true) {
                Happy_ending = true;
                win_Sound();
                ending_Sound();
            }
        }

        //카메라 
        cx = Robot_X, cy = Robot_Y + 0.35, cz = Robot_Z + 0.000001;
        c2x = Robot_X, c2y = Robot_Y + 0.3, c2z = Robot_Z;
        cameraPos = glm::vec3(cx, cy, cz);
        cameraDirection = glm::vec3(c2x, c2y, c2z);
      
        break;
    case 'a':
        //로봇 왼쪽으로 걷기 
        if (current == '0') {
            //회전시킬 것  
            Angle = glm::radians(270.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));

        }
        else if (current == 's') {
            //회전시킬 것
            Angle = glm::radians(270.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));

        }
        else if (current == 'd') {
            //회전시킬 것
            Angle = glm::radians(180.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));

        }
        else if (current == 'w') {
            //회전시킬 것 
            Angle = glm::radians(90.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        TT_Robot = glm::translate(TT_Robot, glm::vec3(-0.005, 0, 0));

        current = 'a';

        Robot_X -= 0.005;
        RobotLX -= 0.005;
        RobotRX -= 0.005;

        if (Floor_state == 2) {
            for (int i = 0; i < 20; ++i) {
                for (int p = 0; p < 20; ++p) {
                    if (Stage2[i][p] == 1) {
                        if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                            || Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall1()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall2()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall3()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall4()) == true)
                        {
                            TT_Robot = glm::translate(TT_Robot, glm::vec3(0.005, 0, 0));
                            Robot_X += 0.005;
                            RobotLX += 0.005;
                            RobotRX += 0.005;
                        }
                    }
                }
            }
            if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(tel_x, tel_z)) == true) {
                R_y = 0.1f;
                Floor_state = 1;
                tel_check = true;
                Robot_Y = 0.1;
                Down_Sound();
            }
        }
        else if (Floor_state == 1) {
            for (int i = 0; i < 20; ++i) {
                for (int p = 0; p < 20; ++p) {
                    if (Stage1[i][p] == 1) {
                        if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                            || Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall1()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall2()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall3()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall4()) == true)
                        {
                            TT_Robot = glm::translate(TT_Robot, glm::vec3(0.005, 0, 0));
                            Robot_X += 0.005;
                            RobotLX += 0.005;
                            RobotRX += 0.005;
                        }
                    }
                }
            }
            if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(box_x, box_z)) == true) {
                Happy_ending = true;
                win_Sound();
                ending_Sound();
            }
        }

        //카메라 
        cx = Robot_X, cy = Robot_Y + 0.35, cz = Robot_Z + 0.000001;
        c2x = Robot_X, c2y = Robot_Y + 0.3, c2z = Robot_Z;
        cameraPos = glm::vec3(cx, cy, cz);
        cameraDirection = glm::vec3(c2x, c2y, c2z);
       
        break;
    case 'd':
        //로봇 오른쪽으로 걷기 
        if (current == '0') {
            //회전시킬 것  
            Angle = glm::radians(90.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        else if (current == 's') {
            //회전시킬 것
            Angle = glm::radians(90.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        else if (current == 'a') {
            //회전시킬 것 
            Angle = glm::radians(180.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        else if (current == 'w') {
            //회전시킬 것 
            Angle = glm::radians(270.0);
            TR_Robot = glm::translate(TR_Robot, glm::vec3(-0.325, 0.52, 0.5));
            TR_Robot = glm::rotate(TR_Robot, Angle, glm::vec3(0, 1, 0));
            TR_Robot = glm::translate(TR_Robot, glm::vec3(0.325, -0.52, -0.5));
        }
        TT_Robot = glm::translate(TT_Robot, glm::vec3(0.005, 0, 0));

        current = 'd';

        Robot_X += 0.005;
        RobotLX += 0.005;
        RobotRX += 0.005;

        if (Floor_state == 2) {
            for (int i = 0; i < 20; ++i) {
                for (int p = 0; p < 20; ++p) {
                    if (Stage2[i][p] == 1) {
                        if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                            || Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall1()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall2()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall3()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall4()) == true)
                        {
                            TT_Robot = glm::translate(TT_Robot, glm::vec3(-0.005, 0, 0));
                            Robot_X -= 0.005;
                            RobotLX -= 0.005;
                            RobotRX -= 0.005;
                        }
                    }
                }
            }
            if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(tel_x, tel_z)) == true) {
                R_y = 0.1f;
                Floor_state = 1;
                tel_check = true;

                Robot_Y = 0.1;
                Down_Sound();
            }
        }
        else if (Floor_state == 1) {
            for (int i = 0; i < 20; ++i) {
                for (int p = 0; p < 20; ++p) {
                    if (Stage1[i][p] == 1) {
                        if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(CubeCenterX[i][p], CubeCenterZ[i][p])) == true
                            || Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall1()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall2()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall3()) == true ||
                            Collide(getbb_monster(Robot_X, Robot_Z), getbb_wall4()) == true)
                        {
                            TT_Robot = glm::translate(TT_Robot, glm::vec3(-0.005, 0, 0));
                            Robot_X -= 0.005;
                            RobotLX -= 0.005;
                            RobotRX -= 0.005;
                        }
                    }
                }
            }
            if (Collide(getbb_robot(Robot_X, Robot_Z), getbb_cube(box_x, box_z)) == true) {
                Happy_ending = true;
                win_Sound();
                ending_Sound();
            }
        }

        //카메라 
        cx = Robot_X, cy = Robot_Y + 0.35, cz = Robot_Z + 0.000001;
        c2x = Robot_X, c2y = Robot_Y + 0.3, c2z = Robot_Z;
        cameraPos = glm::vec3(cx, cy, cz);
        cameraDirection = glm::vec3(c2x, c2y, c2z);
      
        break;
    case 'Q':
        puts("- Program End - ");
        exit(-1);
    }
} 

GLvoid TimerFunction(int value) {
    if (over_check) {
        Clearness += 0.01f;
    }
    if (happy_check) {
        happy_z -= 0.01f;
        if (happy_z < 0.0) {
            happy_check = false;
            happy_z = 0.0f;
        }
    }
    glutTimerFunc(50, TimerFunction, 1);
    glutPostRedisplay();
}
 
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GL_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("TermProject");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }
    else
        cout << "GLEW Initialized\n";

    background_Sound(); 
    UserFunc();
    make_vertexShaders();
    make_fragmentShaders();

    InitTexture();
    InitBuffer();
    InitMonster();
    shaderID = make_shaderProgram();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(myKeyBoard);
    glutTimerFunc(50, LeftArm, 0);
    glutTimerFunc(50, RightArm, 0);
    glutTimerFunc(50, LeftLeg, 0);
    glutTimerFunc(50, RightLeg, 0);
    glutTimerFunc(50, TimerFunction, 0);

    //몬스터 자동으로 움직이는 타이머 콜백 함수  
    glutTimerFunc(30, Move, 0);
   

    glutMainLoop();

    for (int i = 0; i < 20; i++) {
        delete MDataArr[i];
    }
    for (int i = 0; i < 20; i++) {
        delete MDataArr2[i];
    }
    delete[] MDataArr;  
    delete[] MDataArr2; 
}