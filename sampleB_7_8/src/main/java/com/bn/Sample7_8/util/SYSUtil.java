package com.bn.Sample7_8.util;

import javax.vecmath.Quat4f;

public class SYSUtil 
{
	public static float[] fromSYStoAXYZ(Quat4f q4)
	{	
		if(q4.w==1.0f)
		{
			return new float[]{0,1,1,0};
		}
		
		double sitaHalf=Math.acos(q4.w);
		float nx=(float) (q4.x/Math.sin(sitaHalf));
		float ny=(float) (q4.y/Math.sin(sitaHalf));
		float nz=(float) (q4.z/Math.sin(sitaHalf));
		
		return new float[]{(float) Math.toDegrees(sitaHalf*2),nx,ny,nz};
	}

}
