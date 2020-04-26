#version 300 es
precision mediump float;				//����Ĭ�ϵĸ��㾫��

precision mediump float;
uniform int uisShadowFrag;//�Ƿ������Ӱ
uniform int uisLanbanFrag;//�Ƿ�������������ϵ���Ӱ
uniform sampler2D usTextureBall;//����������������

in vec2 vTextureCoord; //���մӶ�����ɫ�������Ĳ���
in vec4 vambient;
in vec4 vdiffuse;
in vec4 vspecular;
in vec4 vfragLosition;
out vec4 fragColor;
void main()                         
{    
   //�����������ɫ����ƬԪ
   vec4 finalColor;
   if(uisShadowFrag==1)
   {//������Ӱ������������������
    finalColor=vec4(0.3,0.3,0.3,0.2);
   	if(uisLanbanFrag==1)
   	{
      if(vfragLosition.x<-9.6||vfragLosition.x>9.6||vfragLosition.y<4.24||vfragLosition.y>10.5)
      {
        finalColor=vec4(0.3,0.3,0.3,0.0);
      }
    }
     
     fragColor = finalColor;//����ƬԪ��ɫֵ
   }
   else
   {
   //����������������������
     finalColor=texture(usTextureBall, vTextureCoord);
     fragColor = finalColor*vambient+finalColor*vspecular+finalColor*vdiffuse;//����ƬԪ��ɫֵ
   }
}