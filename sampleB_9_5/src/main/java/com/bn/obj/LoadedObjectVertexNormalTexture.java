package com.bn.obj;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import com.bn.bnggdh.BNModel;
import com.bn.bnggdh.BnggdhDraw;
import com.bn.bnggdh.LoadTextrueUtil;
import com.bn.bnggdh.MatrixState;
import com.bn.bnggdh.MySurfaceView;
import com.bn.bnggdh.ShaderUtil;

import android.content.res.Resources;
import android.opengl.GLES30;

//加载后的物体――仅携带顶点信息，颜色随机
public class LoadedObjectVertexNormalTexture {
	int mProgram;// 自定义渲染管线着色器程序id
	int muMVPMatrixHandle;// 总变换矩阵引用
	int muMMatrixHandle;// 位置、旋转变换矩阵
	int maPositionHandle; // 顶点位置属性引用
	int maNormalHandle; // 顶点法向量属性引用
	int maLightLocationHandle;// 光源位置属性引用
	int maCameraHandle; // 摄像机位置属性引用
	int maTexCoorHandle; // 顶点纹理坐标属性引用
	String mVertexShader;// 顶点着色器代码脚本
	String mFragmentShader;// 片元着色器代码脚本

	FloatBuffer mVertexBuffer;// 顶点坐标数据缓冲
	FloatBuffer mNormalBuffer;// 顶点法向量数据缓冲
	FloatBuffer mTexCoorBuffer;// 顶点纹理坐标数据缓冲
	int vCount = 0;

	int texId;

	public LoadedObjectVertexNormalTexture(String fname, String picName,
			Resources r, MySurfaceView mv) {
		this.texId = LoadTextrueUtil.initTextureRepeat(mv, picName);
		LoadUtil.loadFromFile(fname, r, mv);
		// 初始化顶点坐标与着色数据
		initVertexData(LoadUtil.m_vertices, LoadUtil.m_normals,
				LoadUtil.m_texCoors);
		// 初始化shader
		initShader(mv);
	}
	
	private BNModel m_bd;
	private String m_boneId;
	private float m_x;
	private float m_y;
	private float m_z;
	private float m_angX;
	private float m_angY;
	private float m_angZ;
	private float m_scaleX;
	private float m_scaleY;
	private float m_scaleZ;
	/**
	 * 
	 * @param bd 		模型类
	 * @param boneId	骨骼id
	 * @param x			x轴上平移的单位
	 * @param y			y轴上平移的单位
	 * @param z			z轴上平移的单位
	 * @param angX		绕x轴旋转angX角度(角度值)
	 * @param angY		绕y轴旋转angY角度(角度值)
	 * @param angZ		绕z轴旋转angZ角度(角度值)
	 * @param scaleX	x轴缩放scaleX(默认为1.0f)
	 * @param scaleY	y轴缩放scaleY(默认为1.0f)
	 * @param scaleZ	z轴缩放scaleZ(默认为1.0f)
	 */
	public void initWithBone(BNModel bd, String boneId, 
			float x, float y, float z, 
			float angX, float angY, float angZ, 
			float scaleX, float scaleY, float scaleZ){
		this.m_bd = bd;
		this.m_boneId = boneId;
		this.m_x = x;
		this.m_y = y;
		this.m_z = z;
		this.m_angX = angX;
		this.m_angY = angY;
		this.m_angZ = angZ;
		this.m_scaleX = scaleX;
		this.m_scaleY = scaleY;
		this.m_scaleZ = scaleZ;
	}

	// 初始化顶点坐标与着色数据的方法
	public void initVertexData(float[] vertices, float[] normals,
			float texCoors[]) {
		// 顶点坐标数据的初始化================begin============================
		vCount = vertices.length / 3;

		// 创建顶点坐标数据缓冲
		// vertices.length*4是因为一个整数四个字节
		ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
		vbb.order(ByteOrder.nativeOrder());// 设置字节顺序
		mVertexBuffer = vbb.asFloatBuffer();// 转换为Float型缓冲
		mVertexBuffer.put(vertices);// 向缓冲区中放入顶点坐标数据
		mVertexBuffer.position(0);// 设置缓冲区起始位置
		// 特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
		// 转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
		// 顶点坐标数据的初始化================end============================

		// 顶点法向量数据的初始化================begin============================
		ByteBuffer cbb = ByteBuffer.allocateDirect(normals.length * 4);
		cbb.order(ByteOrder.nativeOrder());// 设置字节顺序
		mNormalBuffer = cbb.asFloatBuffer();// 转换为Float型缓冲
		mNormalBuffer.put(normals);// 向缓冲区中放入顶点法向量数据
		mNormalBuffer.position(0);// 设置缓冲区起始位置
		// 特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
		// 转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
		// 顶点着色数据的初始化================end============================

		// 顶点纹理坐标数据的初始化================begin============================
		ByteBuffer tbb = ByteBuffer.allocateDirect(texCoors.length * 4);
		tbb.order(ByteOrder.nativeOrder());// 设置字节顺序
		mTexCoorBuffer = tbb.asFloatBuffer();// 转换为Float型缓冲
		mTexCoorBuffer.put(texCoors);// 向缓冲区中放入顶点纹理坐标数据
		mTexCoorBuffer.position(0);// 设置缓冲区起始位置
		// 特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
		// 转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
		// 顶点纹理坐标数据的初始化================end============================
	}

	// 初始化shader
	public void initShader(MySurfaceView mv) {
		// 加载顶点着色器的脚本内容
		mVertexShader = ShaderUtil.loadFromAssetsFile("vertex_light.sh",
				mv.getResources());
		// 加载片元着色器的脚本内容
		mFragmentShader = ShaderUtil.loadFromAssetsFile("frag_light.sh",
				mv.getResources());
		// 基于顶点着色器与片元着色器创建程序
		mProgram = ShaderUtil.createProgram(mVertexShader, mFragmentShader);
		// 获取程序中顶点位置属性引用
		maPositionHandle = GLES30.glGetAttribLocation(mProgram, "aPosition");
		// 获取程序中顶点颜色属性引用
		maNormalHandle = GLES30.glGetAttribLocation(mProgram, "aNormal");
		// 获取程序中总变换矩阵引用
		muMVPMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");
		// 获取位置、旋转变换矩阵引用
		muMMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMMatrix");
		// 获取程序中光源位置引用
		maLightLocationHandle = GLES30.glGetUniformLocation(mProgram,
				"uLightLocation");
		// 获取程序中顶点纹理坐标属性引用
		maTexCoorHandle = GLES30.glGetAttribLocation(mProgram, "aTexCoor");
		// 获取程序中摄像机位置引用
		maCameraHandle = GLES30.glGetUniformLocation(mProgram, "uCamera");
	}
	
	public void drawSelfWithBone(){
		MatrixState.pushMatrix();
		float[] matrix = m_bd.getMatrix(m_boneId);
		MatrixState.matrix(matrix);
		MatrixState.scale(m_scaleX, m_scaleY, m_scaleZ);
		MatrixState.rotate(m_angX, 1, 0, 0);
		MatrixState.rotate(m_angY, 0, 1, 0);
		MatrixState.rotate(m_angZ, 0, 0, 1);
		MatrixState.translate(m_x, m_y, m_z);
		drawSelf();
		MatrixState.popMatrix();
	}

	public void drawSelf() {
		// 制定使用某套着色器程序
		GLES30.glUseProgram(mProgram);
		// 将最终变换矩阵传入着色器程序
		GLES30.glUniformMatrix4fv(muMVPMatrixHandle, 1, false,
				MatrixState.getFinalMatrix(), 0);
		// 将位置、旋转变换矩阵传入着色器程序
		GLES30.glUniformMatrix4fv(muMMatrixHandle, 1, false,
				MatrixState.getMMatrix(), 0);
		// 将光源位置传入着色器程序
		GLES30.glUniform3fv(maLightLocationHandle, 1,
				MatrixState.lightPositionFB);
		// 将摄像机位置传入着色器程序
		GLES30.glUniform3fv(maCameraHandle, 1, MatrixState.cameraFB);
		// 将顶点位置数据传入渲染管线
		GLES30.glVertexAttribPointer(maPositionHandle, 3, GLES30.GL_FLOAT,
				false, 3 * 4, mVertexBuffer);
		// 将顶点法向量数据传入渲染管线
		GLES30.glVertexAttribPointer(maNormalHandle, 3, GLES30.GL_FLOAT, false,
				3 * 4, mNormalBuffer);
		// 将顶点纹理坐标数据传入渲染管线
		GLES30.glVertexAttribPointer(maTexCoorHandle, 2, GLES30.GL_FLOAT,
				false, 2 * 4, mTexCoorBuffer);
		// 启用顶点位置、法向量、纹理坐标数据
		GLES30.glEnableVertexAttribArray(maPositionHandle);
		GLES30.glEnableVertexAttribArray(maNormalHandle);
		GLES30.glEnableVertexAttribArray(maTexCoorHandle);
		// 绑定纹理
		GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texId);
		// 绘制加载的物体
		GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, vCount);
	}
}