#if OPENGL
	#define SV_POSITION POSITION
	#define VS_SHADERMODEL vs_3_0
	#define PS_SHADERMODEL ps_3_0
#else
	#define VS_SHADERMODEL vs_4_0_level_9_1
	#define PS_SHADERMODEL ps_4_0_level_9_1
#endif

matrix WorldViewProjection;

sampler s0 : register (s0);
float Time;
float2 textureSize; 

Texture2D NoiseTexture;
sampler2D NoiseSampler = sampler_state 
{ 
	Texture = <NoiseTexture>; 
};

float4 MainPS(float2 coords: TEXCOORD0) : COLOR0
{
    float2 up_limit = float2(0.0, 0.0) + float2(Time, Time);
    float2 down_limit = textureSize - float2(Time, Time);
    
    float4 output_col; 
    if (coords.x < up_limit.x || coords.y < up_limit.y) {
        output_col = float4(0.0, 0.0, 0.0, 0.0);
        return output_col;
    }
    
    if (coords.x > down_limit.x || coords.y > down_limit.y) {
        output_col = float4(0.0, 0.0, 0.0, 0.0);
        return output_col;
    }
    
    return tex2D(s0, coords);
}

technique BasicColorDrawing
{
	pass P0
	{
		PixelShader = compile PS_SHADERMODEL MainPS();
	}
};