
matrix		g_matWorld, g_matView, g_matProj;

// 1. ������ �����̽���ȯ�� �ݵ�� ����������Ѵ�.
// 2. 
texture		g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};



struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
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
	float3		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In/*�ȼ�*/)
{
	PS_OUT	Out = (PS_OUT)0;

	Out.vColor = vector(1.f, 0.1f, 0.1f, 0.5f);
	//Out.vColor = tex2D(DiffuseSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_MAIN_WARNING(PS_IN In/*�ȼ�*/)
{
	PS_OUT	Out = (PS_OUT)0;

	//Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor = vector(0.35f, 0.35f, 0.35f, 1.f);

	if (((In.vTexUV.y + In.vTexUV.x) * 10 + 10) % 4 > 2)
		Out.vColor = vector(0.75f, 0.75f, 0.35f, 1.f);

	if (In.vTexUV.y < -0.4f || In.vTexUV.y > 0.4f ||
		In.vTexUV.x < -0.4f || In.vTexUV.x > 0.4f/* ||
		In.vTexUV.z < -0.4f || In.vTexUV.z > 0.4f*/)
		Out.vColor = vector(0.35f, 0.35f, 0.35f, 1.f);

	//pow(Out.vColor.rgb, 10.f);

	return Out;
}

PS_OUT PS_MAIN3(PS_IN In/*�ȼ�*/)
{
	PS_OUT	Out = (PS_OUT)0;

	//Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor = vector(0.7f, 0.1f, 0.1f, 1.f);


	return Out;
}




technique Default_Technique
{
	// ���� ǥ���ϰ����ϴ� ������Ǥ� ����.(���, ����ŧ��, �׸���, ������Ʈ, ��Ǻ�)
	pass Red
	{
		cullmode = ccw;
		aLPHAbLENDeNABLE = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Warning
	{
		cullmode = ccw;
		aLPHAbLENDeNABLE = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_WARNING();
	}
	//pass Frendly_ShipInFog
	//{
	//	aLPHAbLENDeNABLE = true;
	//	SrcBlend = SrcAlpha;
	//	DestBlend = InvSrcAlpha;

	//	cullmode = CCW;

	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAIN2();
	//}

	//pass Enemy_Ship_Rendering
	//{
	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAIN3();
	//}

	//aLPHAbLENDeNABLE = true;
	//SrcBlend = SrcAlpha;
	//DestBlend = InvSrcAlpha;
	//cullmode = CCW;

	//// ���ĺ��� + �ְ�
	//pass AlphaBlending
	//{
	//	VertexShader = ;
	//	PixelShader = ;
	//}
}