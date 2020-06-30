
matrix		g_matWorld, g_matView, g_matProj;

// 1. 정점의 스페이스변환을 반드시 수행해줘야한다.
// 2. 

texture g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT/*정점*/ VS_MAIN(VS_IN In/*정점*/)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

// w나누기.(원근투영)

// 뷰포트변환.

// 래스터라이즈.

struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In/*픽셀*/)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = vector(0.85f, 0.f, 0.f, 0.5f);

	if (In.vTexUV.x < 0.1f ||
		In.vTexUV.y < 0.1f ||
		In.vTexUV.x > 0.9 ||
		In.vTexUV.y > 0.9)
		Out.vColor = vector(1.f, 1.f, 1.f, 0.5f);

	return Out;
}

PS_OUT PS_MAIN_GREEN(PS_IN In/*픽셀*/)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = vector(0.f, 0.85f, 0.f, 0.5f);

	if (In.vTexUV.x < 0.1f ||
		In.vTexUV.y < 0.1f ||
		In.vTexUV.x > 0.9 ||
		In.vTexUV.y > 0.9)
		Out.vColor = vector(1.f, 1.f, 1.f, 0.5f);

	return Out;
}

PS_OUT PS_GOAL(PS_IN In/*픽셀*/)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.r = 1;
	Out.vColor.g = 1;
	Out.vColor.b = 0.3;

	return Out;
}




technique Default_Technique
{
	pass Disabled
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
		//fillmode = wireframe;
	}
	pass Enabled
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_GREEN();
		cullmode = none;
	}
	pass Goal
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_GOAL();
		cullmode = none;
	}
}