#include "Mountain.h"
#include "util/ShaderUtil.h"
#include "util/MatrixState.h"
#include "util/FileUtil.h"
#include <math.h>
#include <malloc.h>

#define STRINGIFY(A) #A

Mountain::Mountain()
{
    initVertexData();
    initShader();
}

void Mountain::initVertexData()
{
	int vsize = sizeof(mHigh);
	int rows = sqrt((double)vsize/sizeof(float))-1;
	int cols = rows;

    vCount = cols*cols*6;

	float* vertices = (float*)malloc(vCount*3*sizeof(float));

    int count=0;//���������
     for(int j=0;j<rows;j++)
     {
     	for(int i=0;i<cols;i++)
     	{
    		//���㵱ǰ�������ϲ������
     		float zsx=-UNIT_SIZE*cols/2+i*UNIT_SIZE;
     		float zsz=-UNIT_SIZE*rows/2+j*UNIT_SIZE;

     		vertices[count++]=zsx;
     		vertices[count++]=mHigh[j][i]*LAND_HIGHEST/255-LAND_HIGH_ADJUST;
     		vertices[count++]=zsz;

     		vertices[count++]=zsx;
     		vertices[count++]=mHigh[j+1][i]*LAND_HIGHEST/255-LAND_HIGH_ADJUST;
     		vertices[count++]=zsz+UNIT_SIZE;

     		vertices[count++]=zsx+UNIT_SIZE;
     		vertices[count++]=mHigh[j][i+1]*LAND_HIGHEST/255-LAND_HIGH_ADJUST;
     		vertices[count++]=zsz;

     		vertices[count++]=zsx+UNIT_SIZE;
     		vertices[count++]=mHigh[j][i+1]*LAND_HIGHEST/255-LAND_HIGH_ADJUST;
     		vertices[count++]=zsz;

     		vertices[count++]=zsx;
     		vertices[count++]=mHigh[j+1][i]*LAND_HIGHEST/255-LAND_HIGH_ADJUST;
     		vertices[count++]=zsz+UNIT_SIZE;

     		vertices[count++]=zsx+UNIT_SIZE;
     		vertices[count++]=mHigh[j+1][i+1]*LAND_HIGHEST/255-LAND_HIGH_ADJUST;
     		vertices[count++]=zsz+UNIT_SIZE;
     	}
     }

     mVertexBuffer = &vertices[0];
     float* tex = (float*)malloc(vCount*2*sizeof(float));

     generateTexCoor(cols,cols,tex);
     mTexCoorBuffer = tex;
}

void Mountain::initShader()
{
	string vertexShader=FileUtil::loadShaderStr("shader/vert.sh");
	string fragmentShader=FileUtil::loadShaderStr("shader/frag.sh");

	mProgram = ShaderUtil::createProgram(vertexShader.c_str(), fragmentShader.c_str());
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    //��ȡ�����ж�������������������
	maTexCoorHandle= glGetAttribLocation(mProgram, "aTexCoor");
	//��ȡ�������ܱ任��������
	muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
	//����
	//�ݵ�
	sTextureGrassHandle=glGetUniformLocation(mProgram, "sTextureGrass");
	//ʯͷ
	sTextureRockHandle=glGetUniformLocation(mProgram, "sTextureRock");
	//xλ��
	landStartYYHandle=glGetUniformLocation(mProgram, "landStartY");
	//x���
	landYSpanHandle=glGetUniformLocation(mProgram, "landYSpan");
}

void Mountain::drawSelf(const GLint texId,const GLint rock_textId)
{
    glUseProgram(mProgram);
    
    glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());

	//���Ͷ���λ�����ݽ���Ⱦ����
	glVertexAttribPointer
	(
		maPositionHandle,
		3,
		GL_FLOAT,
		GL_FALSE,
		3*4,
		mVertexBuffer
	);
	//���Ͷ��������������ݽ���Ⱦ����
	glVertexAttribPointer
	(
		maTexCoorHandle,
		2,
		GL_FLOAT,
		GL_FALSE,
		2*4,
		mTexCoorBuffer
	);

	//������λ����������
	glEnableVertexAttribArray(maPositionHandle);
	glEnableVertexAttribArray(maTexCoorHandle);

	//������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rock_textId);
	glUniform1i(sTextureGrassHandle, 0);//ʹ��0������
	glUniform1i(sTextureRockHandle, 1); //ʹ��1������

	//������Ӧ��x����
	glUniform1f(landStartYYHandle, 0);
	glUniform1f(landYSpanHandle, 10);

	//�����������
	glDrawArrays(GL_TRIANGLES, 0, vCount);

	glDisableVertexAttribArray(maPositionHandle);
	glDisableVertexAttribArray(maTexCoorHandle);
}

//�Զ��з����������������ķ���
void Mountain::generateTexCoor(int bw,int bh,float* tex){
    float sizew=16.0f/bw;//����
    float sizeh=16.0f/bh;//����
    int c=0;
    for(int i=0;i<bh;i++){
        for(int j=0;j<bw;j++){
            //ÿ����һ�����Σ������������ι��ɣ��������㣬12����������
            float s=j*sizew;
            float t=i*sizeh;
            tex[c++]=s;
            tex[c++]=t;
            tex[c++]=s;
            tex[c++]=t+sizeh;
            tex[c++]=s+sizew;
            tex[c++]=t;
            tex[c++]=s+sizew;
            tex[c++]=t;
            tex[c++]=s;
            tex[c++]=t+sizeh;
            tex[c++]=s+sizew;
            tex[c++]=t+sizeh;
    }}
}
