package com.bn.Sample7_8;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

import com.bn.Sample7_8.Constant;
public class GameActivity extends Activity {
    MySurfaceView mf;
	public SensorManager mySensorManager;	//SensorManager对象引用	
	public Sensor myAccelerometer; 	//传感器类型
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //设置为全屏
        requestWindowFeature(Window.FEATURE_NO_TITLE); 
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,  
		              WindowManager.LayoutParams.FLAG_FULLSCREEN);
		//设置为横屏模式
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    	//获得SensorManager对象
        mySensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);	        
        myAccelerometer=mySensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		
		MySurfaceView mf=new MySurfaceView(this);
		mf.requestFocus();//获取焦点
		mf.setFocusableInTouchMode(true);//设置为可触控
		setContentView(mf);//跳转欢迎界面
    }
	   //开发实现了SensorEventListener接口的传感器监听器
	  	public SensorEventListener mySensorListener = new SensorEventListener(){
	  		public void onAccuracyChanged(Sensor sensor, int accuracy) {

	  		}
	  		public void onSensorChanged(SensorEvent event) 
	  		{
	  			//计算出重力在屏幕上的投影方向
	  			float []values=event.values;//获取三个轴方向上的加速度值
	  			//System.out.println("v[0]="+values[0]+",,v[1]="+values[1]+",,v[2]="+values[2]);
	  			Constant.Angle=values[1];
	  		}
	  	};
	  	@Override
	  	protected void onResume() {
			super.onResume();
			
	    	mySensorManager.registerListener(
					mySensorListener, 		//添加监听
					myAccelerometer, 		//传感器类型
					SensorManager.SENSOR_DELAY_NORMAL	//传感器事件传递的频度
					);
		}
	    @Override
	    protected void onPause() {
	        super.onPause();
    	mySensorManager.unregisterListener(mySensorListener);//取消注册监听器
	}
}
	