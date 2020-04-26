package com.bn.bullet;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.view.MotionEvent;

import static com.bn.bullet.HelloBulletActivity.*;

@SuppressLint("NewApi")
public class GL2JNIView extends GLSurfaceView
{
	//private final float TOUCH_SCALE_FACTOR = 180.0f/800;//角度缩放比例
	float cx=0;//摄像机x位置
	float cy=150;//摄像机y位置
	float cz=400;//摄像机z位置

    float mPreviousX;
    float mPreviousY;
    
    float xAngle=45;//摄像机绕X轴旋转的角度
    float yAngle=0;//太阳灯光绕y轴旋转的角度
    
    boolean isAddBody;
	SceneRenderer mRenderer;
	public GL2JNIView(Context context)
	{
		super(context);
		this.setEGLContextClientVersion(3);
		mRenderer=new SceneRenderer();
		this.setRenderer(mRenderer);
		this.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
	}
	private class SceneRenderer implements GLSurfaceView.Renderer
	{
		public void onDrawFrame(GL10 gl)
		{
			JNIPort.step();
		}
		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			JNIPort.onSurfaceChanged(width, height);
		}
		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
			JNIPort.onSurfaceCreated(GL2JNIView.this);
		}
	}
	@SuppressLint("ClickableViewAccessibility") @Override
	public boolean onTouchEvent(MotionEvent event)
	{
		float x=event.getX();
		float y=event.getY();
		switch(event.getAction())
		{
			case MotionEvent.ACTION_DOWN:
				isAddBody = true;
				
//				System.out.println(WIDTH/2+" "+HEIGHT/2);
//				System.out.println(x+" "+y);
				
				if(y>HEIGHT/2)
				{
					if(x<WIDTH/3*1)
					{
//						System.out.println("后 左转");
						JNIPort.addBody(1);//后 左转
					}
					else if(x>WIDTH/3*2)
					{
//						System.out.println("后 右转");
						JNIPort.addBody(2);//后 右转
					}
					else
					{
//						System.out.println("后 倒");
						JNIPort.addBody(3);//后 倒
					}
				}
				else
				{
					if(x<WIDTH/3*1)
					{
//						System.out.println("前左转");
						JNIPort.addBody(4);//前左转
					}
					else if(x>WIDTH/3*2)
					{
//						System.out.println("前 右转");
						JNIPort.addBody(5);//前 右转
					}
					else
					{
//						System.out.println("前进");
						JNIPort.addBody(6);//前进
					}
				}
				break;
			case MotionEvent.ACTION_UP:
				if(isAddBody)
				{

					isAddBody = false;
				}
				
				JNIPort.addBody(7);
				break;
			case MotionEvent.ACTION_MOVE:
	            
				break;
		}
		mPreviousX = x;
		mPreviousY = y;
		return true;
	}
	
	//加载纹理的方法
	public static int initTextureRepeat(GLSurfaceView gsv,String pname)
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
		InputStream is = null;
		try {
			is = gsv.getResources().getAssets().open(pname);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
        Bitmap bitmapTmp;
        try {
        	bitmapTmp = BitmapFactory.decodeStream(is);
        } 
        finally {
            try {
                is.close();
            } 
            catch(IOException e) {
                e.printStackTrace();
            }
        }
        //实际加载纹理
        GLUtils.texImage2D
        (
        		GLES30.GL_TEXTURE_2D,   //纹理类型，在OpenGL ES中必须为GL10.GL_TEXTURE_2D
        		0, 					  	//纹理的层次，0表示基本图像层，可以理解为直接贴图
        		bitmapTmp, 			  	//纹理图像
        		0					  	//纹理边框尺寸
        );
        bitmapTmp.recycle(); 		  	//纹理加载成功后释放图片 
        return textureId;
	}
}