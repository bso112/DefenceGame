#pragma once

// 1. 내가 로드해야할 다수의 이미지를 모아 놓는다.
// 2. Ctexture클래스르 ㄹ객체화했다. == 플레이어의 대시에 사용되는 이미지들. 
// 3. Ctexture클래스르 ㄹ객체화했다. == 플레이어의 아이들에 사용되는 이미지들. 
#include "Component.h"

BEGIN(Engine)

class CShader;
class ENGINE_DLL CTexture final : public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	HRESULT Set_TextureOnShader(CShader* pShader, D3DXHANDLE hParameter, _uint iIndex = 0);
public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT Ready_Component(void* pArg);
private:	
	vector<PDIRECT3DTEXTURE9>			m_Textures;
	typedef vector<PDIRECT3DTEXTURE9>	TEXTURES;
public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END