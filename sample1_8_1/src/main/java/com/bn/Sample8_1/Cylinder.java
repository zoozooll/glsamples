package com.bn.Sample8_1;
//Բ����
public class Cylinder
{
	Circle bottomCircle;//��Բ
	Circle topCircle;//��Բ
	CylinderSide cylinderSide;//����
	float xAngle=0;//��x����ת�ĽǶ�
    float yAngle=0;//��y����ת�ĽǶ�
    float zAngle=0;//��z����ת�ĽǶ�
    float h;
    float scale;

    int topTexId; //��������
    int BottomTexId;  //��������
    int sideTexId;  //��������
    
	public Cylinder(MySurfaceView mySurfaceView,float scale,float r, float h,int n,
			int topTexId, int BottomTexId, int sideTexId)
	{
		
		this.h=h;
		this.scale=scale;
		this.topTexId=topTexId;
		this.BottomTexId=BottomTexId;
		this.sideTexId=sideTexId;
		
		topCircle=new Circle(mySurfaceView,scale,r,n);	//��������Բ����
		bottomCircle=new Circle(mySurfaceView,scale,r,n);  //��������Բ����
		cylinderSide=new CylinderSide(mySurfaceView,scale,r,h,n); //���������޶�Բ������
	}
	public void drawSelf()
	{
		MatrixState.rotate(xAngle, 1, 0, 0);
		MatrixState.rotate(yAngle, 0, 1, 0);
		MatrixState.rotate(zAngle, 0, 0, 1);		
		//���ƶ���
		MatrixState.pushMatrix();//�����ֳ�
		MatrixState.translate(0, h/2*scale, 0);//ƽ�Ʊ任
		MatrixState.rotate(-90, 1, 0, 0);//��ת�任
		topCircle.drawSelf(topTexId);//����Բ��
		MatrixState.popMatrix();//�ָ��ֳ�
		
		//���Ƶ���
		MatrixState.pushMatrix();//�����ֳ�
		MatrixState.translate(0, -h/2*scale, 0);//ƽ�Ʊ任
		MatrixState.rotate(90, 1, 0, 0);//��ת�任
		MatrixState.rotate(180, 0, 0, 1);//��ת�任
		bottomCircle.drawSelf(BottomTexId);//����Բ��
		MatrixState.popMatrix();//�ָ��ֳ�
		
		//���Ʋ���
		MatrixState.pushMatrix();//�����ֳ�
		MatrixState.translate(0, -h/2*scale, 0);//ƽ�Ʊ任
		cylinderSide.drawSelf(sideTexId);//��������
		MatrixState.popMatrix();//�ָ��ֳ�
	}
}
