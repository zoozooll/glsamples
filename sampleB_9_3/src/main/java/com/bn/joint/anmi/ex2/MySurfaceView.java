package com.bn.joint.anmi.ex2;
import java.io.InputStream;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.view.MotionEvent;
import android.opengl.GLES30;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import com.bn.ms3d.core.MS3DModel;
import com.bn.ms3d.texutil.TextureManager;
import android.content.Context;
public class MySurfaceView extends GLSurfaceView{
	private final float TOUCH_SCALE_FACTOR = 180.0f/320;//角度缩放比例
    private SceneRenderer mRenderer;//场景渲染器	
	private float mPreviousY;//上次的触控位置Y坐标
    private float mPreviousX;//上次的触控位置X坐标
    float cx=0,cy=7,cz=-15;//摄像机位置
    float tx=0,ty=2,tz=0;//目标点
    float ux=0,uy=2,uz=0;//UP向量
    float angle=45;//摄像机仰角
    float direction=0;//摄像机方位角
    final float distance=25;//摄像机距离目标点的距离
	public MySurfaceView(Context context){
        super(context);
        this.setEGLContextClientVersion(2); //设置使用OPENGL ES2.0
        mRenderer = new SceneRenderer();	//创建场景渲染器
        setRenderer(mRenderer);				//设置渲染器		        
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);//设置渲染模式为主动渲染 
        calCamera(0,0);
    }
	public void calCamera(float dx,float dy) //计算摄像机的位置与旋转角度
	{
		direction += dx * TOUCH_SCALE_FACTOR;	//方位角
		angle+=dy * TOUCH_SCALE_FACTOR;			//方向角
		if(angle<=0)
		{
			angle=0;
		}
		else if(angle>=90)
		{
			angle=90;
		}
        
		//虚拟向量
	   	 double angleHD=Math.toRadians(direction);
	   	 float[] cup={-(float) Math.sin(angleHD),0,(float) Math.cos(angleHD),1};
	   	 float[] cLocation={0,distance,0,1};
	   	 
	   	 //计算转轴向量    	 
	   	 float[] zhou={-cup[2],0,cup[0]};   
	   	
	   	 //计算摄像机参数
	   	 float[] helpM=new float[16];
	   	 Matrix.setIdentityM(helpM, 0);
	   	 Matrix.rotateM(helpM, 0, angle, zhou[0],zhou[1],zhou[2]);
	   	 float[] cupr=new float[4];
	   	 float[] cLocationr=new float[4];
	   	 Matrix.multiplyMV(cupr, 0, helpM, 0, cup, 0);
	   	 Matrix.multiplyMV(cLocationr, 0, helpM, 0, cLocation, 0);
	   	 
	   	 cx=cLocationr[0];//摄像机位置
	   	 cy=cLocationr[1];
	   	 cz=cLocationr[2];
	   	 tx=0f;ty=0f;tz=0f;//观察点位置
	   	 ux=cupr[0];uy=cupr[1];uz=cupr[2];//up向量
	}
	
	//触摸事件回调方法
    @Override 
    public boolean onTouchEvent(MotionEvent e) 
    {
    	float y = e.getY();
        float x = e.getX();
        switch (e.getAction()) 
        {
           case MotionEvent.ACTION_MOVE:
	            float dy = y - mPreviousY;//计算触控笔Y位移
	            float dx = x - mPreviousX;//计算触控笔Y位移
	            calCamera(dx,dy);
           break;
        }
        mPreviousY = y;//记录触控笔位置
        mPreviousX = x;//记录触控笔位置
        return true;
    }

	private class SceneRenderer implements GLSurfaceView.Renderer{     	
    	TextureManager manager;	//纹理管理器
    	MS3DModel ms3d;			//ms3d模型
    	float time = 0;			//当前时间（用于动画播放）
        public void onDrawFrame(GL10 gl){ 
        	//清除深度缓冲与颜色缓冲
            GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);        
            //设置摄像机
            MatrixState.setCamera(
        		cx,cy,cz, 
            	tx,ty,tz, 
            	ux,uy,uz
            );
            final float span=6.0f;
            final int k=2;
            for(int i=-k;i<=k;i++){
            	for(int j=-k;j<=k;j++){
            		MatrixState.pushMatrix();
            		MatrixState.translate(i*span, 0, j*span);
                    this.ms3d.animate(time); 
                    MatrixState.popMatrix();
            	}}
            //更新模型动画时间
            time += 0.015f;
            //若当前播放时间大于总的动画时间则实际播放时间等于当前播放时间减去总的动画时间
            if(time > this.ms3d.getTotalTime()) {
            	time = time - this.ms3d.getTotalTime();
            }}  
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height); 
        	//计算GLSurfaceView的宽高比
            float ratio = (float) width / height;
            //调用此方法计算产生透视投影矩阵
            MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 1,1000);
            //初始化基本变换矩阵
            MatrixState.setInitStack();
        }
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            //设置屏幕背景色RGBA
            GLES30.glClearColor(0.0f,0.0f,0.0f,1.0f);    
            //打开深度检测
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);
            //关闭背面剪裁   
            GLES30.glDisable(GLES30.GL_CULL_FACE);
            //创建纹理管理器对象
            manager = new TextureManager(getResources());
            //获取ms3d文件的输入流
            InputStream in=null;
            try{
            	in=getResources().getAssets().open("ninja.ms3d");
            }
            catch(Exception e)
            {
            	e.printStackTrace();
            }
            //从输入流加载模型
    		ms3d = MS3DModel.load(in,manager,MySurfaceView.this); 	
        }
    }
}