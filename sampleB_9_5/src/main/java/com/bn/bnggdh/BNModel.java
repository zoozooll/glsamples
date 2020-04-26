package com.bn.bnggdh;

import java.io.IOException;
import java.io.InputStream;

public class BNModel {

	private float time = 0;//当前时间
	private float onceTime;// 一次动画所需的时间
	private float interval = 2.0f;// 一组动画 和 一组动画 的间隔时间
	private boolean isNormal;//是否有法向量
	private float dt;//步长
	private float dtFactor;//速率

	private BnggdhDraw cd;// 带法向量的模型类
	private BnggdhDrawNoNormal cdnn;// 不带法向量的模型类

	/**
	 * 
	 * @param sourceName	模型名称
	 * @param picName		图片名称
	 * @param isNormal		是否有光照
	 * @param dtFactor		速率，范围在0-1
	 * @param r				资源类引用
	 */
	public BNModel(String sourceName, String picName, boolean isNormal,
			float dtFactor, MySurfaceView mv) {
		String path = "bnggdh/" + sourceName;
		try {
			InputStream is = mv.getResources().getAssets().open(path);
			if (isNormal == true) {
				cd = new BnggdhDraw(is, mv, picName);
				onceTime = cd.maxKeytime;
			} else {
				cdnn = new BnggdhDrawNoNormal(is, mv, picName);
				onceTime = cdnn.maxKeytime;
			}
			this.dtFactor = dtFactor;
			this.dt = dtFactor * onceTime;
			this.isNormal = isNormal;
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 绘制方法
	 */
	public void draw() {
		System.out.println("time = " + time);
		MatrixState.pushMatrix();
//		MatrixState.translate(0, -70, 0);
//		MatrixState.scale(0.8f, 0.8f, 0.8f);
		if (isNormal == true) {
			cd.draw(time);
		} else {
			cdnn.draw(time);
		}
		time += dt;// 更新模型动画时间
		// 若当前播放时间大于总的动画时间则实际播放时间等于当前播放时间减去总的动画时间
		if (time >= (onceTime + dt + interval)) {
			time = 0;
		}
		MatrixState.popMatrix();
	}

	/**
	 * 获取速率
	 * @return
	 */
	public float getDtFactor() {
		return dtFactor;
	}

	/**
	 * 设置速率
	 * @param dtFactor
	 */
	public void setDtFactor(float dtFactor) {
		if(dtFactor > 0 && dtFactor < 1){
			this.dtFactor = dtFactor;
			this.dt = dtFactor * onceTime;
		}
	}

	/**
	 * 设置当前时间
	 * @param time
	 */
	public void setTime(float time){
		if(time >= 0 && time <= this.onceTime){
			this.time = time;
		}
	}
	
	/**
	 * 获取当前时间
	 * @return
	 */
	public float getTime(){
		return this.time;
	}
	
	/**
	 * 获取该模型的骨骼动画总的时间
	 * @return
	 */
	public float getOnceTime(){
		return this.onceTime;
	}
	
	public float[] getMatrix(String id){
		if (isNormal == true) {
			return cd.bnggdh.getMatrix(id);
		} else {
			return cdnn.bnggdh.getMatrix(id);
		}
	}
}