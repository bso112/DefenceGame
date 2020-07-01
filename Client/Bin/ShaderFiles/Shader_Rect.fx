
matrix		g_matWorld, g_matView, g_matProj;

// 1. ������ �����̽���ȯ�� �ݵ�� ����������Ѵ�.
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

VS_OUT/*����*/ VS_MAIN(VS_IN In/*����*/)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

// w������.(��������)

// ����Ʈ��ȯ.

// �����Ͷ�����.

struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In/*�ȼ�*/)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_WALL(PS_IN In/*�ȼ�*/)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.a *= 0.5f;

	return Out;
}

PS_OUT PS_GOAL(PS_IN In/*�ȼ�*/)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vColor.r = 1;
	Out.vColor.g = 1;
	Out.vColor.b = 0.3;
	
	return Out;
}

PS_OUT PS_RED(PS_IN In/*�ȼ�*/)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.g *= 0.f;
	Out.vColor.b *= 0.f;

	return Out;
}

PS_OUT PS_GREEN(PS_IN In/*�ȼ�*/)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor.r *= 0.f;
	Out.vColor.b *= 0.f;

	return Out;
}



technique Default_Technique
{
	pass Default_Rendering
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
		//fillmode = wireframe;
	}
	pass Wall
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_WALL();
		cullmode = none;
	}
	pass Goal
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_GOAL();
		cullmode = none;
	}

	pass Red
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_RED();
		cullmode = none;
	}

	pass Greed
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_GREEN();
		cullmode = none;
	}
}