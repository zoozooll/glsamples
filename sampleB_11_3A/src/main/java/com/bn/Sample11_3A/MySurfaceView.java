package com.bn.Sample11_3A;//声明包
import java.io.IOException;
import java.io.InputStream;

import android.opengl.GLSurfaceView;
import android.opengl.GLES30;
import android.opengl.GLUtils;
import android.util.Log;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

class MySurfaceView extends GLSurfaceView 
{
	private final float TOUCH_SCALE_FACTOR = 180.0f/320;//角度缩放比例
    private SceneRenderer mRenderer;//场景渲染器 
    private float mPreviousX;//上次的触控位置X坐标
    
    int textureId;//系统分配的纹理id
    int textureJBId;//小草颜色渐变id
    
    float num=40000;//小草的总棵数
   	public  float r=(float) Math.sqrt(num)/4-1;	//摄像机到目标点的距离，即摄像机旋转的半径
	
    //摄像机目标点的坐标
  	public  float targetX=r;
  	public  float targetY=0;
  	public  float targetZ=r;
  	
  	//摄像机的up向量
  	public static float upX=0;
  	public static float upY=1;
  	public static float upZ=0;	
  
  	public static float ANGLE_MIN=-360;//摄像机旋转的角度范围的最小值
  	public static float ANGLE_MAX=360;//摄像机旋转的角度范围的最大值
  	
  	//摄像机的观察者坐标
  	public  float CameraX=r;
  	public  float CameraY=6;
  	public  float CameraZ=r;
  	
    float cAngle=0;//摄像机转动的角度
	
	public MySurfaceView(Context context) {
        super(context);
        this.setEGLContextClientVersion(3); //设置使用OPENGL ES3.0
        mRenderer = new SceneRenderer();	//创建场景渲染器
        setRenderer(mRenderer);				//设置渲染器		        
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);//设置渲染模式为主动渲染   
    }
	
	//触摸事件回调方法
    @Override 
    public boolean onTouchEvent(MotionEvent e) 
    {
        float x = e.getX();
        switch (e.getAction()) 
        {
        case MotionEvent.ACTION_MOVE:
  			float dx = x - mPreviousX;//计算触控点X位移
  			cAngle += dx*TOUCH_SCALE_FACTOR;	
  		}
  		mPreviousX = x;//记录触控点位置
  		calculateCamera(cAngle);//计算摄像机的观察点
  		
        return true;
    
    }

    public  void calculateCamera(float angle)
  	{		
  		//计算摄像机观察者的坐标
      	CameraX = (float) (r*Math.sin(Math.toRadians(angle)))+targetX;
      	CameraZ = (float) (r*Math.cos(Math.toRadians(angle)))+targetZ;
      	//设置摄像机的9参数
      	MatrixState.setCamera(CameraX,CameraY,CameraZ,  targetX,targetY,targetZ,  upX,upY,upZ);
  	}
    
	private class SceneRenderer implements GLSurfaceView.Renderer 
    {  
    	//从指定的obj文件中加载的对象
		GrassObject lovo;
		GrassGroup grassGroup;//小草对象组
		
		//测试刷帧频率的代码
		long olds;
		long currs;
		
		public void onDrawFrame(GL10 gl) 
		{
			//注释掉的为测试刷帧频率的代码
			currs=System.nanoTime();
			Log.d("FPS", (1000000000.0/(currs-olds)+"FPS"));
			olds=currs;
			//清除深度缓冲与颜色缓冲
			GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);
            grassGroup.drawGroup(textureId,textureJBId);                
        }  

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height); 
        	//计算GLSurfaceView的宽高比
            float ratio = (float) width / height;
            //调用此方法计算产生透视投影矩阵
            MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 2, 10000);
            //调用此方法产生摄像机9参数位置矩阵
            calculateCamera(cAngle);//计算摄像机的观察点并设置摄像机的位置
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) 
        {
            //设置屏幕背景色RGBA
            GLES30.glClearColor(0.5f,0.5f,0.5f,1.0f);    
            //打开深度检测
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);
            //初始化变换矩阵
            MatrixState.setInitStack();
            //加载要绘制的物体
            lovo=LoadUtil.loadFromFile("grass.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
            grassGroup=new GrassGroup(lovo,(int) num);
            //加载纹理图
            textureId=initTexture(R.drawable.grass);
            textureJBId=initTexture(R.drawable.jb);
        }
    }
  	public int initTexture(int drawableId)//textureId
	{
		//生成纹理ID
		int[] textures = new int[1];
		GLES30.glGenTextures
		(
				1,          //产生的纹理id的数量
				textures,   //纹理id的数组
				0           //偏移量
		);    
		int textureId=textures[0];    
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_NEAREST);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D,GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_REPEAT);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_REPEAT);
        
        //通过输入流加载图片===============begin===================
        InputStream is = this.getResources().openRawResource(drawableId);
        Bitmap bitmapTmp;
        try 
        {
        	bitmapTmp = BitmapFactory.decodeStream(is);
        } 
        finally 
        {
            try 
            {
                is.close();
            } 
            catch(IOException e) 
            {
                e.printStackTrace();
            }
        }
        //通过输入流加载图片===============end===================== 
	   	GLUtils.texImage2D
	    (
	    		GLES30.GL_TEXTURE_2D, //纹理类型
	     		0, 
	     		GLUtils.getInternalFormat(bitmapTmp), 
	     		bitmapTmp, //纹理图像
	     		GLUtils.getType(bitmapTmp), 
	     		0 //纹理边框尺寸
	     );
	    bitmapTmp.recycle(); 		  //纹理加载成功后释放图片
        return textureId;
	}
}