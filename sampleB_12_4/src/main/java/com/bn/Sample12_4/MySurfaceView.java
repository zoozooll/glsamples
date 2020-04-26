package com.bn.Sample12_4;//声明包

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import android.opengl.GLSurfaceView;
import android.opengl.GLES30;
import android.opengl.GLUtils;
import android.os.Environment;
import android.util.Log;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.bn.Sample12_4.R;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap.Config;
import android.graphics.Matrix;

class MySurfaceView extends GLSurfaceView 
{
    private SceneRenderer mRenderer;//场景渲染器    
    public static boolean threadFlag=false;
    
    int screenWidth;
    int screenHeight;
    boolean saveFlag=false;
    int textureId;//系统分配的纹理id
	
    int BUTTON_LEFT=1580;
    int BUTTON_RIGHT=1780;
    int BUTTON_UP=450;
    int BUTTON_DOWN=620;
    float ratio;
    float xAngle=0;//自转角度
    float yAngle=0;//自转角度
    
    Matrix matrix = new Matrix();
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
		float x=Constant.fromRealScreenXToStandardScreenX(e.getX());//将当前屏幕坐标转换为标准屏幕坐标
		float y=Constant.fromRealScreenYToStandardScreenY(e.getY());
        switch (e.getAction()) {
        case MotionEvent.ACTION_DOWN:
        	if(x>BUTTON_LEFT&&x<BUTTON_RIGHT&&y>BUTTON_UP&&y<BUTTON_DOWN)
        	{
        		setFlag(true);
        	}
        }
        return true;
    }
    public synchronized void setFlag(boolean flag)
    {
    	saveFlag=flag;
    }
	private class SceneRenderer implements GLSurfaceView.Renderer 
    {  
    	//从指定的obj文件中加载的对象
		LoadedObjectVertexNormalTexture lovo;
		Triangle texRect;//纹理矩形
		int texTriId;
        public void onDrawFrame(GL10 gl) 
        { 
        	//清除深度缓冲与颜色缓冲
            GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);

            //坐标系推远
            MatrixState.pushMatrix();
            MatrixState.translate(-8f, -8f, -50f);
            //地球自转
            MatrixState.rotate(yAngle, 0, 1, 0);
            MatrixState.rotate(xAngle, 1, 0, 0);
            
            //若加载的物体不为空则绘制物体
            if(lovo!=null)
            {
            	lovo.drawSelf(textureId);
            }   
            MatrixState.popMatrix(); 
            
            texRect.drawSelf(texTriId);
            
            if(saveFlag)
            {
            	saveScreen();
            	setFlag(false);
            }
        }  
        public void saveScreen()
        {
            matrix.reset();  
            matrix.setRotate(180); //旋转180度
            matrix.postScale(-1, 1);
        	
        	ByteBuffer cbbTemp = ByteBuffer.allocateDirect(screenWidth*screenHeight*4);
        	GLES30.glReadPixels(0, 0, screenWidth, screenHeight, GLES30.GL_RGBA, GLES30.GL_UNSIGNED_BYTE, cbbTemp);
        	
        	Bitmap bm =Bitmap.createBitmap(screenWidth, screenHeight, Config.ARGB_8888);
        	bm.copyPixelsFromBuffer(cbbTemp);
        	bm=Bitmap.createBitmap(bm, 0, 0, screenWidth, screenHeight, matrix, true);
        	try
			{
	    		File sd=Environment.getExternalStorageDirectory();
	    		String path=sd.getPath()+"/HappyScreenShot";
	    		File file=new File(path);
	    		if(!file.exists())
	    		{
	    			file.mkdirs();
	    		}
        		File myFile = File.createTempFile
        				(
        						"ScreenShot",  //基本文件名
        						".png",     //后缀
        						file //目录路径
        				);
        		
				 FileOutputStream fout=new FileOutputStream(myFile);
				 BufferedOutputStream bos = new BufferedOutputStream(fout);  
				 bm.compress
				 (
						 Bitmap.CompressFormat.PNG,   //图片格式
						 100, 						   //品质0-100
						 bos						   //使用的输出流
				  );   
				 bos.flush();   
				 bos.close(); 
				 System.out.println("保存成功，文件名："+myFile.getName());
			}
			catch(Exception e)
			{
				e.printStackTrace();
				System.out.println("保存失败！"); 
			}
        }
        public void onSurfaceChanged(GL10 gl, int width, int height) {
        	screenWidth=width;
        	screenHeight=height;
            //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height); 
        	//计算GLSurfaceView的宽高比
            ratio = (float) width / height;
            //调用此方法计算产生透视投影矩阵
            MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 1, 70);
            //调用此方法产生摄像机9参数位置矩阵
            MatrixState.setCamera(0,0,3,0f,0f,-1f,0f,1.0f,0.0f);
            //启动一个线程定时旋转地球、月球
            new Thread()
            {
            	public void run()
            	{
            		while(threadFlag)
            		{
            			//地球自转角度
            			xAngle=(xAngle+2)%360;
            			yAngle=(yAngle+3)%360;
            			try {
							Thread.sleep(100);
						} catch (InterruptedException e) {				  			
							e.printStackTrace();
						}
            		}
            	}
            }.start();   
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) 
        {
            Log.d("SystemInfo", "GL_RENDERER="+gl.glGetString(GL10.GL_RENDERER));
            Log.d("SystemInfo", "GL_VENDOR="+gl.glGetString(GL10.GL_VENDOR));
            Log.d("SystemInfo", "GL_VERSION="+gl.glGetString(GL10.GL_VERSION));
            Log.i("SystemInfo", "GL_EXTENSIONS="+gl.glGetString(GL10.GL_EXTENSIONS));
            //设置屏幕背景色RGBA
            GLES30.glClearColor(0.0f,0.0f,0.0f,1.0f);    
            //打开深度检测
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);
            //打开背面剪裁   
            GLES30.glEnable(GLES30.GL_CULL_FACE);
            //初始化变换矩阵
            MatrixState.setInitStack();
            //初始化光源位置
            MatrixState.setLightLocation(40, 10, 20);
            //加载要绘制的物体
            lovo=LoadUtil.loadFromFile("ch_t.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
            //加载纹理图
            textureId=initTexture(R.drawable.ghxp);
            //创建三角形对对象 
            texRect=new Triangle(MySurfaceView.this);
            texTriId=initTexture(R.drawable.screenshot1);
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