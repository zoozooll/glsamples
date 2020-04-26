    #version 300 es
    precision mediump float;
    uniform sampler2D sTexture;
    in vec2 vTextureCoord;
    out vec4 fragColor;
    void main()
    {
	    vec4 gColor = texture(sTexture, vTextureCoord);
	    fragColor = gColor;
    }
    