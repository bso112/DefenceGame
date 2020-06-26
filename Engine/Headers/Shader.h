#pragma once

// 셰이더파일 하나를 컴파일하여 사용할 수 있도록 한다.
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CShader(const CShader& rhs);
	virtual ~CShader() = default;
public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pFilePath);
	virtual HRESULT Ready_Component(void* pArg);
public:
	HRESULT Set_Value(D3DXHANDLE hParameter, void* pData, _uint iSize);
	//dds 파일과 jpg, png 파일의 로드방식은 다르기때문에 텍스쳐들의 상위인터페이스인 IDirect3DBaseTexture9으로 받는다.
	HRESULT Set_Texture(D3DXHANDLE hParameter, IDirect3DBaseTexture9* pTexture);
	
public:
	HRESULT Begin_Shader();
	HRESULT End_Shader();
	HRESULT Begin_Pass(_uint iIndex);
	HRESULT End_Pass();

private:
	LPD3DXEFFECT			m_pEffect = nullptr;	

	
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END