package com.bn.bnggdh;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.bn.obj.LoadedObjectVertexNormalTexture;

import android.annotation.SuppressLint;
import android.content.Context;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.view.MotionEvent;

@SuppressLint("ClickableViewAccessibility")
public class MySurfaceView extends GLSurfaceView {
	private final float TOUCH_SCALE_FACTOR = 180.0f / 320;// 角度缩放比例
	private SceneRenderer mRenderer;// 场景渲染器
	private float mPreviousY;// 上次触摸点位置Y坐标
	private float mPreviousX;// 上次触摸点位置X坐标

	float angle = 45;// 摄像机仰角
	float direction = 0;// 摄像机方位角
	final float distance = 120;// 摄像机距离目标点的距离

	// y轴上 1
	float cx = 0, cy = distance, cz = 0;// 摄像机位置
	float tx = 0, ty = 0, tz = 0;// 目标点
	float ux = 0, uy = 0, uz = 1;// up向量

	public MySurfaceView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		this.setEGLContextClientVersion(3);// 设置使用OpenGL ES3.0
		mRenderer = new SceneRenderer();// 创建场景渲染器
		setRenderer(mRenderer);// 设置渲染器
		setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);// 设置渲染模式为连续不断的
		// calCamera(0,0);
	}

	// 计算摄像机的位置与旋转角度
	public void calCamera(float dx, float dy) {
		direction += dx * TOUCH_SCALE_FACTOR;// 方位角
		angle += dy * TOUCH_SCALE_FACTOR;// 方向角
		if (angle <= 0) {
			angle = 0;
		} else if (angle >= 90) {
			angle = 90;
		}
		// 虚拟向量
		double angleHD = Math.toRadians(direction);
		float[] cup = { -(float) Math.sin(angleHD), 0,
				(float) Math.cos(angleHD), 1 };
		float[] cLocation = { 0, distance, 0, 1 };

		// 计算旋转向量
		float[] zhou = { -cup[2], 0, cup[0] };

		// 计算摄像机参数
		float[] helpM = new float[16];
		Matrix.setIdentityM(helpM, 0);
		Matrix.rotateM(helpM, 0, angle, zhou[0], zhou[1], zhou[2]);// 矩阵 根据角度 和
																	// 轴向量 进行旋转
		float[] cupr = new float[4];
		float[] cLocationr = new float[4];
		Matrix.multiplyMV(cupr, 0, helpM, 0, cup, 0);// 旋转矩阵 和 xz面上的轴向量 求出up向量
		Matrix.multiplyMV(cLocationr, 0, helpM, 0, cLocation, 0);// 旋转矩阵 和
																	// y轴上的一个向量
																	// 求出
																	// 摄像机位置坐标

		// 摄像机位置
		cx = cLocationr[0];
		cy = cLocationr[1];
		cz = cLocationr[2];

		// 观察点位置
		tx = 0f;
		ty = 50;
		tz = 0f;

		// up向量
		ux = cupr[0];
		uy = cupr[1];
		uz = cupr[2];
	}

	@Override
	public boolean onTouchEvent(MotionEvent e) {
		// TODO Auto-generated method stub
		float y = e.getY();
		float x = e.getX();
		switch (e.getAction()) {
		case MotionEvent.ACTION_MOVE:
			float dy = y - mPreviousY;
			float dx = x - mPreviousX;
			calCamera(dx, dy);// 设置摄像机位置及相关参数
			break;
		}
		mPreviousY = y;
		mPreviousX = x;
		return true;
	}

	private class SceneRenderer implements GLSurfaceView.Renderer {

		BNModel bnm;
		LoadedObjectVertexNormalTexture lo;

		public void onDrawFrame(GL10 gl) {
			// TODO Auto-generated method stub
			// 清除深度缓冲与颜色缓冲
			GLES30.glClear(GLES30.GL_DEPTH_BUFFER_BIT
					| GLES30.GL_COLOR_BUFFER_BIT);

			MatrixState.setLightLocation(cx, cy, cz);
			// MatrixState.setLightLocation(300, 0, 0);
			// 设置摄像机
			MatrixState.setCamera(cx, cy, cz, tx, ty, tz, ux, uy, uz);

			bnm.draw();
			lo.drawSelfWithBone();
		}

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			// TODO Auto-generated method stub
			// 设置视窗大小及位置
			GLES30.glViewport(0, 0, width, height);
			// 计算GLSurfaceView的宽高比
			float ratio = (float) width / height;
			// 调用此方法计算产生透视投影矩阵
			MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 1, 1000);
			// MatrixState.setProjectOrtho(-ratio, ratio, -1, 1, 1, 1000);
			// 初始化基本变换矩阵
			MatrixState.setInitStack();
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			// TODO Auto-generated method stub
			// 设置屏幕背景色RGBA
			GLES30.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			// 打开深度检测
			GLES30.glEnable(GLES30.GL_DEPTH_TEST);
			// 关闭背面剪裁
			GLES30.glDisable(GLES30.GL_CULL_FACE);
			// GLES30.glEnable(GLES30.GL_CULL_FACE);

			bnm = new BNModel("a.bnggdh", "hero1.png", false, 0.005f, MySurfaceView.this);
			bnm.setDtFactor(0.001f);

			lo = new LoadedObjectVertexNormalTexture("box1.obj", "b.jpg",
					getResources(), MySurfaceView.this);
			lo.initWithBone(bnm, "Bip01 R Finger0", 4, 0, -2, 90, 0, 0, 1,1,1);//Bone03
		}
	}

}