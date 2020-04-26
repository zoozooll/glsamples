package com.bn.bnggdh;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import android.annotation.SuppressLint;
import android.opengl.GLES30;

import com.bn.jar.bnggdh.Bnggdh;

@SuppressLint("UseSparseArrays")
public class BnggdhDrawNoNormal {

	public float maxKeytime;
	private int texId;
	Bnggdh bnggdh;

	public BnggdhDrawNoNormal(InputStream is, MySurfaceView mv, String path) {
		
		bnggdh = new Bnggdh(is);
		try {
			bnggdh.init();
		} catch (IOException e) {
			e.printStackTrace();
		}
		maxKeytime = bnggdh.getMaxKeytime();
		
		this.texId = LoadTextrueUtil.initTextureRepeat(mv, path);
		initShader(mv);
		initBuffer();
	}

	int mProgram;// 自定义渲染管线程序id
	int maPositionHandle;// 顶点位置属性引用id
	int maTexCoorHandle; // 顶点纹理坐标属性引用id
	int muMVPMatrixHandle;// 总变换矩阵引用id
	int muTexHandle;// 纹理属性id

	public void initShader(MySurfaceView mv) {
		// 加载顶点着色器的脚本
		String mVertexShader = ShaderUtil.loadFromAssetsFile("vertex.sh",
				mv.getResources());
		// 加载片元着色器的脚本
		String mFragmentShader = ShaderUtil.loadFromAssetsFile("frag.sh",
				mv.getResources());
		// 创建shader程序
		mProgram = ShaderUtil.createProgram(mVertexShader, mFragmentShader);
		// 获取shader程序中顶点位置属性引用id
		maPositionHandle = GLES30.glGetAttribLocation(mProgram, "aPosition");
		// 获取程序中顶点纹理坐标属性引用id
		maTexCoorHandle = GLES30.glGetAttribLocation(mProgram, "aTexCoor");
		// 获取程序中总变换矩阵引用id
		muMVPMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");
		// 获取纹理属性id
		muTexHandle = GLES30.glGetUniformLocation(mProgram, "sTexture");

	}

	private FloatBuffer mTextureBuffer;// 顶点纹理数据缓冲
	private ShortBuffer mIndexBuffer;// 顶点索引数据缓存
	private int mTextureBufferId;
	private int mIndexBufferId;
	private int mVertexBufferId;// 顶点坐标数据缓冲 id

	ByteBuffer vbb1;// 顶点坐标数据的映射缓冲
	FloatBuffer mVertexMappedBuffer;// 顶点坐标映射缓冲对应的顶点坐标数据缓冲

	private void initBuffer() {
		int bufferIds[] = new int[3];
		GLES30.glGenBuffers(3, bufferIds, 0);
		mTextureBufferId = bufferIds[0];
		mIndexBufferId = bufferIds[1];
		mVertexBufferId = bufferIds[2];

		ByteBuffer mTex_bf = ByteBuffer
				.allocateDirect(bnggdh.getTextures().length * 4);// 创建顶点纹理数据缓冲
		mTex_bf.order(ByteOrder.nativeOrder());// 设置字节顺序
		mTextureBuffer = mTex_bf.asFloatBuffer();// 转换成FloatBuffer
		mTextureBuffer.put(bnggdh.getTextures());// 向 顶点纹理数据缓存区 中放 顶点纹理数据
		mTextureBuffer.position(0);// 设置缓冲区的起始位置
		// 特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
		// 绑定到纹理坐标数据缓冲
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mTextureBufferId);
		// 向纹理坐标数据缓冲送入数据
		GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER,
				bnggdh.getTextures().length * 4, mTextureBuffer,
				GLES30.GL_STATIC_DRAW);

		ByteBuffer mInd_bf = ByteBuffer
				.allocateDirect(bnggdh.getIndices().length * 2);// 创建顶点索引数据缓存
		mInd_bf.order(ByteOrder.nativeOrder());// 设置字节顺序
		mIndexBuffer = mInd_bf.asShortBuffer();// 转换成ShortBuffer
		mIndexBuffer.put(bnggdh.getIndices());// 向 顶点索引数据缓存 中放 顶点索引数据
		mIndexBuffer.position(0);// 设置缓冲区的起始位置
		// 特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
		// 绑定到纹理坐标数据缓冲
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mIndexBufferId);
		// 向纹理坐标数据缓冲送入数据
		GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER,
				bnggdh.getIndices().length * 2, mIndexBuffer,
				GLES30.GL_STATIC_DRAW);

		// 顶点坐标数据的初始化================start============================
		// 绑定到顶点坐标数据缓冲
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVertexBufferId);
		// 向顶点坐标数据缓冲送入数据,分配vertices.length*4个存储单位（通常是字节）的
		// 内存，用于存储顶点数据或索引。以前所有与当前绑定对象相关联的数据都将删除。
		GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER,
				bnggdh.getPosition().length * 4, null, GLES30.GL_STATIC_DRAW);
		vbb1 = (ByteBuffer) GLES30.glMapBufferRange(GLES30.GL_ARRAY_BUFFER, // 表示顶点数据
				0, // 偏移量
				bnggdh.getPosition().length * 4, // 长度
				GLES30.GL_MAP_WRITE_BIT | GLES30.GL_MAP_INVALIDATE_BUFFER_BIT// 访问标志
		);
		if (vbb1 == null) {
			return;
		}
		vbb1.order(ByteOrder.nativeOrder());// 设置字节顺序
		mVertexMappedBuffer = vbb1.asFloatBuffer();// 转换为Float型缓冲

		mVertexMappedBuffer.put(bnggdh.getPosition());// 向映射的缓冲区中放入顶点坐标数据
															// verticesCube
		mVertexMappedBuffer.position(0);// 设置缓冲区起始位置
		if (GLES30.glUnmapBuffer(GLES30.GL_ARRAY_BUFFER) == false) {
			return;
		}
		// 顶点坐标数据的初始化================end============================

		// 绑定到系统默认缓冲 系统的是0 要不然其他正常的就画不出来了
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, 0);
	}

	private void refreshBuffer() {
		// 绑定到顶点坐标数据缓冲
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVertexBufferId);
		vbb1 = (ByteBuffer) GLES30.glMapBufferRange(GLES30.GL_ARRAY_BUFFER, 0, // 偏移量
				bnggdh.getPosition().length * 4, // 长度
				GLES30.GL_MAP_WRITE_BIT | GLES30.GL_MAP_INVALIDATE_BUFFER_BIT// 访问标志
		);
		if (vbb1 == null) {
			return;
		}
		vbb1.order(ByteOrder.nativeOrder());// 设置字节顺序
		mVertexMappedBuffer = vbb1.asFloatBuffer();// 转换为Float型缓冲
		mVertexMappedBuffer.put(bnggdh.getPosition());// 向映射的缓冲区中放入顶点坐标数据
		mVertexMappedBuffer.position(0);// 设置缓冲区起始位置
		if (GLES30.glUnmapBuffer(GLES30.GL_ARRAY_BUFFER) == false) {
			return;
		}
	}

	public void draw(float time) {
		bnggdh.updata(time);
		refreshBuffer();// 更新缓存区

		GLES30.glUseProgram(mProgram);// 使用某套shader程序
		GLES30.glUniformMatrix4fv(muMVPMatrixHandle, 1, false,
				MatrixState.getFinalMatrix(), 0);// 将最终变换矩阵传入shader程序

		GLES30.glEnableVertexAttribArray(maPositionHandle);// 允许顶点位置数据
		GLES30.glEnableVertexAttribArray(maTexCoorHandle);// 允许顶点纹理数据

		// 顶点数据=======first============
		// 绑定到顶点坐标数据缓冲
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVertexBufferId);
		// 将顶点位置数据送入渲染管线
		GLES30.glVertexAttribPointer(maPositionHandle, 3, GLES30.GL_FLOAT,
				false, 3 * 4, 0);
		// 顶点数据=======end============

		// 纹理数据=======first============
		// 绑定到顶点纹理坐标数据缓冲
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mTextureBufferId);
		// 指定顶点纹理坐标数据
		GLES30.glVertexAttribPointer(maTexCoorHandle, 2, GLES30.GL_FLOAT,
				false, 2 * 4, 0);
		// 绑定到系统默认缓冲 系统的是0 要不然其他正常的就画不出来了
		GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, 0);
		// 纹理数据=======end============

		// 绑定纹理
		GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, this.texId);// 第二个参数如果用好几副纹理图的话
																// 应该写
																// this.vdfd.texId[i]
		GLES30.glUniform1i(muTexHandle, 0);

		// 索引数据=======first============
		// 根据索引缓存区来绘制
		GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
		// 以三角形方式执行绘制
		GLES30.glDrawElements(GLES30.GL_TRIANGLES, bnggdh.getIndices().length,
				GLES30.GL_UNSIGNED_SHORT, 0);
		GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, 0);
		// 索引数据=======end============

		GLES30.glDisableVertexAttribArray(maPositionHandle);
		GLES30.glDisableVertexAttribArray(maTexCoorHandle);
	}
}