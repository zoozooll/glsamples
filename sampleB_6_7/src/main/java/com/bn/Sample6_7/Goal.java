package com.bn.Sample6_7;

import com.bn.util.LoadedObjectVertexNormalTexture;


public class Goal {
    LoadedObjectVertexNormalTexture goal;
	public Goal(MysurfaceView mfv,LoadedObjectVertexNormalTexture goal)
	{
		this.goal=goal;
	}
	public void darwSelf(int texId)
	{
		goal.drawSelf(texId);
	}
}
